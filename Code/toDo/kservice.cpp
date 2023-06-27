#include "kservice.h"

KService::KService(QObject *parent)
	: QObject(parent)
	, m_dao(nullptr)
{
	initDao();
	initConnect();
}

KService::~KService()
{
	delete m_dao;
}
// ��ʼ�����ݽӿڲ�
void KService::initDao()
{
	m_dao = new KDatabase(nullptr);
	m_daoMsg = m_dao->m_daoMsg;
}

void KService::initConnect()
{

}
// ��¼
bool KService::login(QString username, QString password)
{
	return m_dao->selectAccount(username, password);
}
// ע��
bool KService::regist(QString username, QString password)
{
	// �ȼ�����ݿ������Ƿ�����ͬ�û���
	if (m_dao->selectAccountByName(username))return false;
	m_dao->transaction();
	// �Ҳ����Ϳ��Բ���������
	if (!m_dao->insertAccount(username, password))
	{
		m_dao->rollback();
		return false;
	}
	if (!m_dao->insertTodoUser(username))
	{
		m_dao->rollback();
		return false;
	}
	m_dao->commit();
	return true;
}
// ɾ������
bool KService::deleteTaskByID(int id)
{
	return m_dao->deleteTaskByID(id);
}
// ɾ���û����ڷ���
bool KService::deleteUsersGroup(int group_id, int user_id, QString username)
{
	m_dao->transaction();
	QVariant member_ids = m_dao->selectGroupsMemberIds(group_id);
	if (member_ids.isNull()) {
		m_dao->rollback();
		return false;
	}
	QVariant groupList = m_dao->selectUsersGroup(username);
	if (groupList.isNull())
	{
		m_dao->rollback();
		return false;
	}
	QStringList group_List = groupList.toString().split(',', QString::SkipEmptyParts);
	group_List.removeOne(QString::number(group_id));
	QString new_list = group_List.join(",");
	if (m_dao->updateUserGroup(user_id, new_list))
	{
		if (member_ids.toString() == QString::number(user_id)) // �����ֻ���Լ�����ôֱ�Ӱ���ɾ��
		{

			if (!m_dao->deleteGroupByID(group_id) || !m_dao->deleteTaskByGroupId(group_id))
			{
				m_dao->rollback();
				return false;
			}	

		}
		// ���б��ˣ���ôֻ�Ƴ��Լ�
		else
		{
			QStringList memberList = member_ids.toString().split(',', QString::SkipEmptyParts);
			memberList.removeOne(QString::number(user_id));
			QString new_members = memberList.join(",");
			if (!m_dao->updateGroupMember(group_id, new_members))
			{
				m_dao->rollback();
				return false;
			}
		}
	}
	else
	{
		m_dao->rollback();
		return false;
	}
	m_dao->commit();
	return true;
	
}
// �½�����
bool KService::createNewTaskByName(QString taskName, int userId)
{
	return m_dao->createNewTaskByName(taskName, userId);
}
// �½�����
bool KService::createNewTaskByGroupId(int group_id, QString task_name)
{
	return m_dao->createNewTaskByGroupId(group_id, task_name);
}
// �½�����
bool KService::createInfo(QString username, int group_id)
{
	int user_id = m_dao->selectUserID(username);
	if (user_id == 0) return false;
	if (m_dao->selectUserInfoByUserIdAndGroupId(user_id, group_id)) // ˵����ǰ�Ѿ�����ͬ�������ˣ��������ظ�����
	{
		return true;
	}
	return m_dao->insertInfo(user_id, group_id);
}
// ��ѯ�û�ID
int KService::selectUserID(QString username)
{
	return m_dao->selectUserID(username);
}
// ���½�ֹ����
bool KService::updateDueDate(int task_id, QDateTime due_date)
{
	return m_dao->updateDueDate(task_id, due_date);
}
// ���´�������
bool KService::taskNameChange(int task_id, QString new_name)
{
	return m_dao->updateTaskName(task_id, new_name);
}
// ���´�������ʱ��
bool KService::updateRemindTime(int task_id, QDateTime remindTime, bool is_repeated)
{
	return m_dao->updateRemindTime(task_id, remindTime, is_repeated);
}
// ���´�������
bool KService::updateDescription(int task_id, QString description)
{
	return m_dao->updateDescription(task_id, description);
}
// ���´���ص�
bool KService::updateLocation(int task_id, QString location)
{
	return m_dao->updateLocation(task_id, location);
}
// ���±�ע
bool KService::updateNote(int task_id, QString note)
{
	return m_dao->updateNote(task_id, note);
}
// �������ȼ�
bool KService::updatePriority(int task_id, int index)
{
	return m_dao->updatePriority(task_id, index);
}
// ���·�������
bool KService::updateGroupName(int group_id, QString newName)
{
	return m_dao->updateGroupNameById(group_id, newName);
}
// ���ܷ�������
bool KService::acceptInfo(int group_id, int user_id, QString username)
{
	m_dao->transaction();
	if (m_dao->deleteInfo(group_id, user_id))
	{
		if (!m_dao->selectHaveGroup(group_id))		// ���⻹û�н�������Է��Ͱ���ɾ�˵����������֮ǰ��һ��������Ƿ񻹴���
		{
			m_dao->commit();
			return true;
		}
		QVariant member_ids = m_dao->selectGroupsMemberIds(group_id);
		if (member_ids.isNull()) {
			m_dao->rollback();
			return false;
		}
		QVariant groupList = m_dao->selectUsersGroup(username);
		QStringList group_List = groupList.toString().split(',', QString::SkipEmptyParts);
		group_List.append(QString::number(group_id));
		QString new_list = group_List.join(",");

		QStringList members_List = member_ids.toString().split(',', QString::SkipEmptyParts);
		members_List.append(QString::number(user_id));
		QString new_members = members_List.join(",");

		if (!m_dao->updateUserGroup(user_id, new_list) ||  !m_dao->updateGroupMember(group_id, new_members))
		{
			m_dao->rollback();
			return false;
		}
	}
	else
	{
		m_dao->rollback();
		return false;
	}
	m_dao->commit();
	return true;
}
// �ܾ���������
bool KService::refuseInfo(int group_id, int user_id)
{
	return m_dao->deleteInfo(group_id, user_id);
}
// �½�����
bool KService::createNewGroup(QString groupName, int user_id, QString username)
{

	m_dao->transaction();
	if (!m_dao->insertGroup(groupName, user_id))
	{
		m_dao->rollback();
		return false;
	}
	QMap<int, QString> groupIdMap = m_dao->selectGroupID(groupName);

	QList<int> groupIds;
	for (auto id : groupIdMap.keys())
	{
		QString userStr = groupIdMap.value(id);
		QStringList users = userStr.split(',', QString::SkipEmptyParts);
		if (users.size() == 1 && users.first().toInt() == user_id)
			groupIds.append(id);
	}
	if (groupIds.isEmpty())
	{
		m_dao->rollback();
		return false;
	}
	int maxId = -1;
	for (const auto& id : groupIds)
	{
		if (id > maxId)
			maxId = id;
	}
	QVariant groupList = m_dao->selectUsersGroup(username);
	if (groupList.isNull())
	{
		groupList = QString::number(maxId);
	}
	else
	{
		groupList = groupList.toString() + "," + QString::number(maxId);
	}
	if (!m_dao->updateUserGroup(user_id, groupList.toString()))
	{
		m_dao->rollback();
		return false;
	}
	m_dao->commit();
	return true;
}
// ��ѯ����
QMap<int, QString> KService::selectGroup(QString username)
{
	QMap<int, QString>map;
	QVariant groupList = m_dao->selectUsersGroup(username);
	if (!groupList.isNull())
	{
		QStringList groupIds = groupList.toString().split(",", QString::SkipEmptyParts);
		for (auto groupIdStr : groupIds) {
			bool ok;
			int groupId = groupIdStr.toInt(&ok);
			if (ok) {
				QString name = m_dao->selectGroupNameById(groupId);
				map.insert(groupId, name);
			}
			else {
				qWarning() << "Invalid group ID:" << groupIdStr;
			}
		}
	}
	return map;
}
// ��ѯ�û��յ�������
QMap<int, QString> KService::selectUserInfo(int user_id)
{
	QMap<int, QString> map;
	QList<int> groupIdList = m_dao->selectUserInfoByUserId(user_id);
	QList<QString> groupName;
	for (const auto& get : groupIdList)
	{
		QString name = m_dao->selectGroupNameById(get);
		if (!name.isEmpty()) // �� isEmpty() �ж��ַ����Ƿ�Ϊ��
		{
			groupName.append(name);
		}
	}

	for (int i = 0; i < groupIdList.size(); ++i)
	{
		map.insert(groupIdList.at(i), groupName.at(i));
	}

	return map;
}
// ��ѯδ�������� ��������
QList<KTask> KService::selectUnscheduledTasks(QString username)
{
	QList<KTask>taskList;
	m_dao->transaction();
	int userID = m_dao->selectUserID(username);
	QVariant groupList = m_dao->selectUsersGroup(username);
	if (!groupList.isNull())
	{
		QStringList groupIds = groupList.toString().split(",", QString::SkipEmptyParts);
		for (auto groupIdStr : groupIds) {
			bool ok;
			int groupId = groupIdStr.toInt(&ok);
			if (ok) {
				QList<KTask> tasks = m_dao->selectTaskUnscheduledGroup(groupId);
				taskList.append(tasks);
			}
			else {
				qWarning() << "Invalid group ID:" << groupIdStr;
			}
		}
	}
	
	if (userID == -1)
	{
		m_dao->rollback();
		taskList.clear();
		return taskList;
	}
	QList<KTask>taskListNew;
	taskListNew = m_dao->selectTaskUnscheduled(userID);
	taskList.append(taskListNew);
	m_dao->commit();
	return taskList;
}
// ��ѯ��7�찲������ ��������
QList<KTask> KService::selectNext7DaysTasks(QString username)
{
	QList<KTask>taskList;
	m_dao->transaction();
	int userID = m_dao->selectUserID(username);
	QVariant groupList = m_dao->selectUsersGroup(username);
	if (!groupList.isNull())
	{
		QStringList groupIds = groupList.toString().split(",", QString::SkipEmptyParts);
		for (auto groupIdStr : groupIds) {
			bool ok;
			int groupId = groupIdStr.toInt(&ok);
			if (ok) {
				QList<KTask> tasks = m_dao->selectTaskNext7DaysGroup(groupId);
				taskList.append(tasks);
			}
			else {
				qWarning() << "Invalid group ID:" << groupIdStr;
			}
		}
	}

	if (userID == -1)
	{
		m_dao->rollback();
		taskList.clear();
		return taskList;
	}
	QList<KTask>taskListNew;
	taskListNew = m_dao->selectTaskNext7Days(userID);
	taskList.append(taskListNew);
	m_dao->commit();
	return taskList;

}
// ��ѯһ�ܺ������� ��������
QList<KTask> KService::selectAfterNextWeekTasks(QString username)
{
	QList<KTask>taskList;
	m_dao->transaction();
	int userID = m_dao->selectUserID(username);
	QVariant groupList = m_dao->selectUsersGroup(username);
	if (!groupList.isNull())
	{
		QStringList groupIds = groupList.toString().split(",", QString::SkipEmptyParts);
		for (auto groupIdStr : groupIds) {
			bool ok;
			int groupId = groupIdStr.toInt(&ok);
			if (ok) {
				QList<KTask> tasks = m_dao->selectTaskAfterNextWeekGroup(groupId);
				taskList.append(tasks);
			}
			else {
				qWarning() << "Invalid group ID:" << groupIdStr;
			}
		}
	}

	if (userID == -1)
	{
		m_dao->rollback();
		taskList.clear();
		return taskList;
	}
	QList<KTask>taskListNew;
	taskListNew = m_dao->selectTaskNextWeek(userID);
	taskList.append(taskListNew);
	m_dao->commit();
	return taskList;
}
// ��ѯ�Ѿ���ɵ����� ��������
QList<KTask> KService::selectHaveFinishedTasks(QString username)
{
	QList<KTask>taskList;
	m_dao->transaction();
	int userID = m_dao->selectUserID(username);
	QVariant groupList = m_dao->selectUsersGroup(username);
	if (!groupList.isNull())
	{
		QStringList groupIds = groupList.toString().split(",", QString::SkipEmptyParts);
		for (auto groupIdStr : groupIds) {
			bool ok;
			int groupId = groupIdStr.toInt(&ok);
			if (ok) {
				QList<KTask> tasks = m_dao->selectHaveFinishedTasksGroup(groupId);
				taskList.append(tasks);
			}
			else {
				qWarning() << "Invalid group ID:" << groupIdStr;
			}
		}
	}
	if (userID == -1)
	{
		m_dao->rollback();
		taskList.clear();
		return taskList;
	}
	QList<KTask>taskListNew;
	taskListNew = m_dao->selectHaveFinishedTasks(userID);
	taskList.append(taskListNew);
	m_dao->commit();
	return taskList;
}
// ��ѯ��������
QList<KTask> KService::selectAllTasks(QString username)
{
	QList<KTask>taskList;
	m_dao->transaction();
	int userID = m_dao->selectUserID(username);
	QVariant groupList = m_dao->selectUsersGroup(username);
	if (!groupList.isNull())
	{
		QStringList groupIds = groupList.toString().split(",", QString::SkipEmptyParts);
		for (auto groupIdStr : groupIds) {
			bool ok;
			int groupId = groupIdStr.toInt(&ok);
			if (ok) {
				QList<KTask> tasks = m_dao->selectAllTasksGroup(groupId);
				taskList.append(tasks);
			}
			else {
				qWarning() << "Invalid group ID:" << groupIdStr;
			}
		}
	}
	if (userID == -1)
	{
		m_dao->rollback();
		taskList.clear();
		return taskList;
	}
	QList<KTask>taskListNew;
	taskListNew = m_dao->selectAllTasks(userID);
	taskList.append(taskListNew);
	m_dao->commit();
	return taskList;
}
// ��ѯ��������
QString KService::selectGroupNameById(int id)
{
	return m_dao->selectGroupNameById(id);
}
