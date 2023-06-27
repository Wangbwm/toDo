#include "kdatabase.h"
/**
 * ���ݷ��ʲ����ʵ�֣��������ݿ�ʵ����ɾ�Ĳ鹤��
 */
KDatabase::KDatabase(QObject *parent)
	: QObject(parent)
{
	Server_conn();
}
// �ر����ݿ�����
KDatabase::~KDatabase()
{
	m_db.close();
}

// ��ʼ������Դ
void KDatabase::Server_conn()
{
	m_db = QSqlDatabase::addDatabase("QMYSQL");
	if (m_db.isValid())
	{
		qDebug() << "ok";
	}
	else
	{
		qDebug() << "error";
	}
	m_db.setHostName("114.132.229.115"); //
	m_db.setPort(3306);       // ���ʹ�õ���Ĭ�϶˿ڿ��Բ�����
	m_db.setUserName("root");	// ���ݿ��û���
	m_db.setPassword("1259963577"); // ���ݿ�����
	m_db.setDatabaseName("todo"); // ���ݿ�����
	if (m_db.open())
	{
		qDebug() << QStringLiteral("���ݿ�򿪳ɹ�, ���Զ�д������......");
		m_daoMsg = QStringLiteral("���ݿ�򿪳ɹ�, ���Զ�д������......");
	}
	else
	{
		qDebug() << QStringLiteral("���ݿ��ʧ��: ") + m_db.lastError().text();
		m_daoMsg = QStringLiteral("���ݿ��ʧ��: ") + m_db.lastError().text();
	}
}

// �����û��������ѯ�û���ʹ��prepare��sql����ʽ����������ֹsqlע��
bool KDatabase::selectAccount(QString username, QString password)
{
	if (!m_db.open()) {
		return false;
	}
	QSqlQuery query;
	//��ֹsqlע��
	QString sqltext = "select * from UserAccount where username = :username and password = :password";
	query.prepare(sqltext);
	query.bindValue(":username", username);
	query.bindValue(":password", password);
	bool ok = query.exec();
	if (!ok) {
		query.lastError().text();
		return false;
	}
	else if (!query.next()) {
		// ��ѯ���Ϊ��
		return false;
	}
	else {
		// ��ѯ����ǿ�
		return true;
	}
}
// �����û�����ѯ�û���ʹ��prepare��sql����ʽ����������ֹsqlע��
bool KDatabase::selectAccountByName(QString username)
{
	if (!m_db.open()) {
		return false;
	}
	QSqlQuery query;
	QString sqltext = "SELECT * FROM UserAccount WHERE username = :username";
	query.prepare(sqltext);
	query.bindValue(":username", username);
	bool ok = query.exec();
	if (!ok) {
		query.lastError().text();
		return false;
	}
	else if (!query.next()) {
		// ��ѯ���Ϊ��
		return false;
	}
	else {
		// ��ѯ����ǿ�
		return true;
	}
	return false;
}

// ��ѯ�û��յ�������Ϣ
bool KDatabase::selectUserInfoByUserIdAndGroupId(int user_id, int group_id)
{
	if (!m_db.open()) {
		return false;
	}
	QSqlQuery query;
	QString sqltext = "SELECT * FROM todo_info WHERE user_id = :user_id AND group_id = :group_id";
	query.prepare(sqltext);
	query.bindValue(":user_id", user_id);
	query.bindValue(":group_id", group_id);
	bool ok = query.exec();
	if (!ok) {
		query.lastError().text();
		return false;
	}
	else if (!query.next()) {
		// ��ѯ���Ϊ��
		return false;
	}
	else {
		// ��ѯ����ǿ�
		return true;
	}
	return false;
}
bool KDatabase::selectHaveGroup(int group_id)
{
	if (!m_db.open()) {
		return false;
	}
	QSqlQuery query;
	QString sqltext = "SELECT * FROM todo_groups WHERE id = :id";
	query.prepare(sqltext);
	query.bindValue(":id", group_id);
	bool ok = query.exec();
	if (!ok) {
		query.lastError().text();
		return false;
	}
	else if (!query.next()) {
		// ��ѯ���Ϊ��
		return false;
	}
	else {
		// ��ѯ����ǿ�
		return true;
	}
	return false;
}
// �½��û�
bool KDatabase::insertAccount(QString username, QString password)
{
	if (!m_db.open()) {
		return false;
	}
	QSqlQuery query;
	QString sqltext = "INSERT INTO UserAccount(username, password) VALUES(:username, :password)";
	query.prepare(sqltext);
	query.bindValue(":username", username);
	query.bindValue(":password", password);
	bool ok = query.exec();
	if (!ok) {
		query.lastError().text();
		return false;
	}
	return true;
}
// �½��û����û�
bool KDatabase::insertTodoUser(QString username)
{
	if (!m_db.open()) {
		return false;
	}
	QSqlQuery query;
	QString sqltext = "INSERT INTO todo_user(username) VALUES(:username)";
	query.prepare(sqltext);
	query.bindValue(":username", username);
	bool ok = query.exec();
	if (!ok) {
		query.lastError().text();
		return false;
	}
	return true;
}
// �½�����
bool KDatabase::insertGroup(QString groupName, int user_id)
{
	if (!m_db.open()) {
		return false;
	}
	QSqlQuery query;
	QString sqltext = "INSERT INTO todo_groups(group_name, member_ids) VALUES(:group_name, :member_ids)";
	query.prepare(sqltext);
	query.bindValue(":group_name", groupName);
	query.bindValue(":member_ids", QString::number(user_id));
	bool ok = query.exec();
	if (!ok) {
		query.lastError().text();
		return false;
	}
	return true;
}
// �½�������Ϣ
bool KDatabase::insertInfo(int user_id, int group_id)
{
	if (!m_db.open()) {
		return false;
	}
	QSqlQuery query;
	QString sqltext = "INSERT INTO todo_info(user_id, group_id) VALUES(:user_id, :group_id)";
	query.prepare(sqltext);
	query.bindValue(":user_id", user_id);
	query.bindValue(":group_id", group_id);
	bool ok = query.exec();
	if (!ok) {
		query.lastError().text();
		return false;
	}
	return true;
}
// ɾ������
bool KDatabase::deleteTaskByID(int id)
{
	if (!m_db.open()) {
		return false;
	}
	QSqlQuery query;
	query.prepare("DELETE FROM todo_list WHERE id = :id");
	query.bindValue(":id", id);
	bool ok = query.exec();
	if (!ok) {
		query.lastError().text();
		return false;
	}
	return true;
}
// ɾ������
bool KDatabase::deleteGroupByID(int id)
{
	if (!m_db.open()) {
		return false;
	}
	QSqlQuery query;
	query.prepare("DELETE FROM todo_groups WHERE id = :id");
	query.bindValue(":id", id);
	bool ok = query.exec();
	if (!ok) {
		query.lastError().text();
		return false;
	}
	return true;
}
// ɾ������
bool KDatabase::deleteTaskByGroupId(int group_id)
{
	if (!m_db.open()) {
		return false;
	}
	QSqlQuery query;
	query.prepare("DELETE FROM todo_list WHERE group_id = :group_id");
	query.bindValue(":group_id", group_id);
	bool ok = query.exec();
	if (!ok) {
		query.lastError().text();
		return false;
	}
	return true;
}
// ɾ����Ϣ
bool KDatabase::deleteInfo(int group_id, int user_id)
{
	if (!m_db.open()) {
		return false;
	}
	QSqlQuery query;
	query.prepare("DELETE FROM todo_info WHERE group_id = :group_id AND user_id = :user_id");
	query.bindValue(":group_id", group_id);
	query.bindValue(":user_id", user_id);
	bool ok = query.exec();
	if (!ok) {
		query.lastError().text();
		return false;
	}
	return true;
}

// �½�����
bool KDatabase::createNewTaskByName(QString taskName, int userId)
{
	if (!m_db.open()) {
		return false;
	}
	QSqlQuery query;
	query.prepare("INSERT INTO todo_list (user_id, event_name, is_arranged, is_repeated) "
		"VALUES (:user_id, :event_name, :is_arranged, :is_repeated)");
	query.bindValue(":user_id", userId);
	query.bindValue(":event_name", taskName);
	query.bindValue(":is_arranged", false);
	query.bindValue(":is_repeated", false);
	bool success = query.exec();
	return success;
}
// �½�����
bool KDatabase::createNewTaskByGroupId(int group_id, QString taskName)
{
	if (!m_db.open()) {
		return false;
	}
	QSqlQuery query;
	query.prepare("INSERT INTO todo_list (group_id, event_name, is_arranged, is_repeated) "
		"VALUES (:group_id, :event_name, :is_arranged, :is_repeated)");
	query.bindValue(":group_id", group_id);
	query.bindValue(":event_name", taskName);
	query.bindValue(":is_arranged", false);
	query.bindValue(":is_repeated", false);
	bool success = query.exec();
	return success;
}
// ���½�ֹ����
bool KDatabase::updateDueDate(int task_id, QDateTime due_date)
{
	if (!m_db.open()) {
		return false;
	}
	QSqlQuery query;
	QString update_sql = "UPDATE todo_list SET is_arranged = :is_arranged, due_date = :due_date WHERE id=:id";
	query.prepare(update_sql);
	query.bindValue(":is_arranged", true);
	query.bindValue(":due_date", due_date.toString("yyyy-MM-dd hh:mm:ss"));
	query.bindValue(":id", task_id);
	bool success = query.exec();
	return success;
}
// ���´�������
bool KDatabase::updateTaskName(int task_id, QString new_name)
{
	if (!m_db.open()) {
		return false;
	}
	QSqlQuery query;
	QString update_sql = "UPDATE todo_list SET event_name = :event_name WHERE id=:id";
	query.prepare(update_sql);
	query.bindValue(":event_name", new_name);
	query.bindValue(":id", task_id);
	bool success = query.exec();
	return success;
}
// ��������ʱ��
bool KDatabase::updateRemindTime(int task_id, QDateTime remindTime, bool is_repeated)
{
	if (!m_db.open()) {
		return false;
	}
	QSqlQuery query;
	QString update_sql = "UPDATE todo_list SET remind_time = :remind_time, is_repeated = :is_repeated WHERE id=:id";
	query.prepare(update_sql);
	query.bindValue(":remind_time", remindTime.toString("yyyy-MM-dd hh:mm:ss"));
	query.bindValue(":is_repeated", is_repeated);
	query.bindValue(":id", task_id);
	bool success = query.exec();
	return success;
}
// ��������
bool KDatabase::updateDescription(int task_id, QString description)
{
	if (!m_db.open()) {
		return false;
	}
	QSqlQuery query;
	QString update_sql = "UPDATE todo_list SET description = :description WHERE id=:id";
	query.prepare(update_sql);
	query.bindValue(":description", description);
	query.bindValue(":id", task_id);
	bool success = query.exec();
	return success;
}
// ���µص�
bool KDatabase::updateLocation(int task_id, QString location)
{
	if (!m_db.open()) {
		return false;
	}
	QSqlQuery query;
	QString update_sql = "UPDATE todo_list SET location = :location WHERE id=:id";
	query.prepare(update_sql);
	query.bindValue(":location", location);
	query.bindValue(":id", task_id);
	bool success = query.exec();
	return success;
}
// ���±�ע
bool KDatabase::updateNote(int task_id, QString note)
{
	if (!m_db.open()) {
		return false;
	}
	QSqlQuery query;
	QString update_sql = "UPDATE todo_list SET note = :note WHERE id=:id";
	query.prepare(update_sql);
	query.bindValue(":note", note);
	query.bindValue(":id", task_id);
	bool success = query.exec();
	return success;
}
// �������ȼ�
bool KDatabase::updatePriority(int task_id, int index)
{
	if (!m_db.open()) {
		return false;
	}
	QSqlQuery query;
	QString update_sql = "UPDATE todo_list SET priority = :priority WHERE id=:id";
	query.prepare(update_sql);
	query.bindValue(":priority", index);
	query.bindValue(":id", task_id);
	bool success = query.exec();
	return success;
}
// �����û�����
bool KDatabase::updateUserGroup(int user_id, QString groups_list)
{
	if (!m_db.open()) {
		return false;
	}
	QSqlQuery query;
	QString update_sql = "UPDATE todo_user SET groups_list = :groups_list WHERE id=:id";
	query.prepare(update_sql);
	query.bindValue(":groups_list", groups_list);
	query.bindValue(":id", user_id);
	bool success = query.exec();
	return success;
}
// ���·����û�
bool KDatabase::updateGroupMember(int group_id, QString mem_list)
{
	if (!m_db.open()) {
		return false;
	}
	QSqlQuery query;
	QString update_sql = "UPDATE todo_groups SET member_ids = :member_ids WHERE id=:id";
	query.prepare(update_sql);
	query.bindValue(":member_ids", mem_list);
	query.bindValue(":id", group_id);
	bool success = query.exec();
	return success;
}
// ���·�������
bool KDatabase::updateGroupNameById(int group_id, QString newName)
{
	if (!m_db.open()) {
		return false;
	}
	QSqlQuery query;
	QString update_sql = "UPDATE todo_groups SET group_name = :group_name WHERE id=:id";
	query.prepare(update_sql);
	query.bindValue(":group_name", newName);
	query.bindValue(":id", group_id);
	bool success = query.exec();
	return success;
}


// ��ѯ�û�����
QVariant KDatabase::selectUsersGroup(QString username)
{
	if (!m_db.open()) {
		return QVariant();
	}

	QSqlQuery query;
	QString sqltext = "SELECT groups_list FROM todo_user WHERE username = :username";
	query.prepare(sqltext);
	query.bindValue(":username", username);

	bool ok = query.exec();
	if (!ok) {
		return QVariant();
	}

	if (query.next()) {
		// ��ȡ��ѯ������󶨵�QVariantʵ����
		QVariant groupsList = query.value(0);
		return groupsList;
	}
	else {
		return QVariant();
	}
}
// ��ѯ����ID
QMap<int, QString> KDatabase::selectGroupID(QString groupName)
{
	QMap<int, QString>map;
	if (!m_db.open()) {
		return map;
	}

	QSqlQuery query;
	QString sqltext = "SELECT id, member_ids FROM todo_groups WHERE group_name = :group_name";
	query.prepare(sqltext);
	query.bindValue(":group_name", groupName);
	bool ok = query.exec();
	if (!ok) {
		return map;
	}
	while (query.next()) {
		// ��ȡ��ѯ����е������ֶ�ֵ
		int id = query.value("id").toInt();
		QString member_ids = query.value("member_ids").toString();
		map.insert(id, member_ids);
	}
	return map;
}
// ��ѯδ����
QList<KTask> KDatabase::selectTaskUnscheduled(int userId)
{
	if (!m_db.open()) {
		return QList<KTask>();
	}

	QList<KTask> taskList;
	QSqlQuery query;

	QString sqlText = "SELECT * FROM todo_list WHERE user_id = :user_id AND is_arranged = 0";
	query.prepare(sqlText);
	query.bindValue(":user_id", userId);

	bool ok = query.exec();
	if (!ok) {
		return QList<KTask>();
	}

	while (query.next()) {
		// ��ȡ��ѯ����е������ֶ�ֵ
		int id = query.value("id").toInt();
		int groupId = -1;
		int userId = query.value("user_id").isNull() ? -1 : query.value("user_id").toInt();
		QString eventName = query.value("event_name").toString();
		bool isArranged = query.value("is_arranged").toBool();
		int priority = query.value("priority").toInt();

		QDateTime dueDate = query.value("due_date").isNull() ? QDateTime() : query.value("due_date").toDateTime();
		bool isRepeated = query.value("is_repeated").toBool();
		QDateTime remindTime = query.value("remind_time").isNull() ? QDateTime() : query.value("remind_time").toDateTime();

		QString description = query.value("description").toString();
		QString location = query.value("location").toString();
		QString note = query.value("note").toString();

		
			KTask task(
				id, userId, eventName, isArranged,
				priority, dueDate, isRepeated, remindTime, description, location, note
			);
			taskList.append(task);
	}

	return taskList;
}
// ��ѯ������
QList<KTask> KDatabase::selectTaskNext7Days(int userId)
{
	if (!m_db.open()) {
		return QList<KTask>();
	}

	QList<KTask> taskList;
	QSqlQuery query;

	QString sqlText = "SELECT * FROM todo_list WHERE user_id = :user_id AND is_arranged = 1 AND due_date BETWEEN :start_date AND :end_date";
	query.prepare(sqlText);
	query.bindValue(":user_id", userId);
	QDateTime startDate = QDateTime::currentDateTime();
	QDateTime endDate = startDate.addDays(7);
	query.bindValue(":start_date", startDate);
	query.bindValue(":end_date", endDate);

	bool ok = query.exec();
	if (!ok) {
		return QList<KTask>();
	}

	while (query.next()) {
		// ��ȡ��ѯ����е������ֶ�ֵ
		int id = query.value("id").toInt();
		int groupId = -1;
		int userId = query.value("user_id").isNull() ? -1 : query.value("user_id").toInt();
		QString eventName = query.value("event_name").toString();
		bool isArranged = query.value("is_arranged").toBool();
		int priority = query.value("priority").toInt();

		QDateTime dueDate = query.value("due_date").isNull() ? QDateTime() : query.value("due_date").toDateTime();
		bool isRepeated = query.value("is_repeated").toBool();
		QDateTime remindTime = query.value("remind_time").isNull() ? QDateTime() : query.value("remind_time").toDateTime();

		QString description = query.value("description").toString();
		QString location = query.value("location").toString();
		QString note = query.value("note").toString();


		KTask task(
			id, userId, eventName, isArranged,
			priority, dueDate, isRepeated, remindTime, description, location, note
		);
		taskList.append(task);
	}

	return taskList;
}
// ��ѯ�����
QList<KTask> KDatabase::selectTaskNextWeek(int userId)
{
	if (!m_db.open()) {
		return QList<KTask>();
	}

	QList<KTask> taskList;
	QSqlQuery query;

	QString sqlText = "SELECT * FROM todo_list WHERE user_id = :user_id AND is_arranged = 1 AND due_date > :end_date";
	query.prepare(sqlText);
	query.bindValue(":user_id", userId);
	QDateTime endDate = QDateTime::currentDateTime().addDays(7);
	query.bindValue(":end_date", endDate);

	bool ok = query.exec();
	if (!ok) {
		return QList<KTask>();
	}

	while (query.next()) {
		// ��ȡ��ѯ����е������ֶ�ֵ
		int id = query.value("id").toInt();
		int groupId = -1;
		int userId = query.value("user_id").isNull() ? -1 : query.value("user_id").toInt();
		QString eventName = query.value("event_name").toString();
		bool isArranged = query.value("is_arranged").toBool();
		int priority = query.value("priority").toInt();

		QDateTime dueDate = query.value("due_date").isNull() ? QDateTime() : query.value("due_date").toDateTime();
		bool isRepeated = query.value("is_repeated").toBool();
		QDateTime remindTime = query.value("remind_time").isNull() ? QDateTime() : query.value("remind_time").toDateTime();

		QString description = query.value("description").toString();
		QString location = query.value("location").toString();
		QString note = query.value("note").toString();


		KTask task(
			id, userId, eventName, isArranged,
			priority, dueDate, isRepeated, remindTime, description, location, note
		);
		taskList.append(task);
	}

	return taskList;
}
// ��ѯ�����
QList<KTask> KDatabase::selectHaveFinishedTasks(int userId)
{
	if (!m_db.open()) {
		return QList<KTask>();
	}

	QList<KTask> taskList;
	QSqlQuery query;

	QString sqlText = "SELECT * FROM todo_list WHERE user_id = :user_id AND is_arranged = 1 AND due_date <= :end_date";
	query.prepare(sqlText);
	query.bindValue(":user_id", userId);
	QDateTime endDate = QDateTime::currentDateTime();
	query.bindValue(":end_date", endDate);

	bool ok = query.exec();
	if (!ok) {
		return QList<KTask>();
	}

	while (query.next()) {
		// ��ȡ��ѯ����е������ֶ�ֵ
		int id = query.value("id").toInt();
		int groupId = -1;
		int userId = query.value("user_id").isNull() ? -1 : query.value("user_id").toInt();
		QString eventName = query.value("event_name").toString();
		bool isArranged = query.value("is_arranged").toBool();
		int priority = query.value("priority").toInt();

		QDateTime dueDate = query.value("due_date").isNull() ? QDateTime() : query.value("due_date").toDateTime();
		bool isRepeated = query.value("is_repeated").toBool();
		QDateTime remindTime = query.value("remind_time").isNull() ? QDateTime() : query.value("remind_time").toDateTime();

		QString description = query.value("description").toString();
		QString location = query.value("location").toString();
		QString note = query.value("note").toString();


		KTask task(
			id, userId, eventName, isArranged,
			priority, dueDate, isRepeated, remindTime, description, location, note
		);
		taskList.append(task);
	}

	return taskList;
}
// ��ѯ���д���
QList<KTask> KDatabase::selectAllTasks(int userId)
{
	if (!m_db.open()) {
		return QList<KTask>();
	}

	QList<KTask> taskList;
	QSqlQuery query;

	QString sqlText = "SELECT * FROM todo_list WHERE user_id = :user_id";
	query.prepare(sqlText);
	query.bindValue(":user_id", userId);

	bool ok = query.exec();
	if (!ok) {
		return QList<KTask>();
	}

	while (query.next()) {
		// ��ȡ��ѯ����е������ֶ�ֵ
		int id = query.value("id").toInt();
		int groupId = -1;
		int userId = query.value("user_id").isNull() ? -1 : query.value("user_id").toInt();
		QString eventName = query.value("event_name").toString();
		bool isArranged = query.value("is_arranged").toBool();
		int priority = query.value("priority").toInt();

		QDateTime dueDate = query.value("due_date").isNull() ? QDateTime() : query.value("due_date").toDateTime();
		bool isRepeated = query.value("is_repeated").toBool();
		QDateTime remindTime = query.value("remind_time").isNull() ? QDateTime() : query.value("remind_time").toDateTime();

		QString description = query.value("description").toString();
		QString location = query.value("location").toString();
		QString note = query.value("note").toString();


		KTask task(
			id, userId, eventName, isArranged,
			priority, dueDate, isRepeated, remindTime, description, location, note
		);
		taskList.append(task);
	}

	return taskList;
}
// ��ѯ����δ���
QList<KTask> KDatabase::selectTaskUnscheduledGroup(int groupId)
{
	if (!m_db.open()) {
		return QList<KTask>();
	}

	QList<KTask> taskList;
	QSqlQuery query;

	QString sqlText = "SELECT * FROM todo_list WHERE group_id = :group_id AND is_arranged = 0";
	query.prepare(sqlText);
	query.bindValue(":group_id", groupId);

	bool ok = query.exec();
	if (!ok) {
		return QList<KTask>();
	}

	while (query.next()) {
		// ��ȡ��ѯ����е������ֶ�ֵ
		int id = query.value("id").toInt();
		int groupId = query.value("group_id").isNull() ? -1 : query.value("group_id").toInt();
		int userId = -1;
		QString eventName = query.value("event_name").toString();
		bool isArranged = query.value("is_arranged").toBool();
		int priority = query.value("priority").toInt();

		QDateTime dueDate = query.value("due_date").isNull() ? QDateTime() : query.value("due_date").toDateTime();
		bool isRepeated = query.value("is_repeated").toBool();
		QDateTime remindTime = query.value("remind_time").isNull() ? QDateTime() : query.value("remind_time").toDateTime();

		QString description = query.value("description").toString();
		QString location = query.value("location").toString();
		QString note = query.value("note").toString();


		KTask task(
			id, groupId, eventName, isArranged,
			priority, dueDate, isRepeated, remindTime, description, location, note, true
		);
		taskList.append(task);
	}

	return taskList;
}
// ��ѯ����7����
QList<KTask> KDatabase::selectTaskNext7DaysGroup(int groupId)
{
	if (!m_db.open()) {
		return QList<KTask>();
	}

	QList<KTask> taskList;
	QSqlQuery query;

	QString sqlText = "SELECT * FROM todo_list WHERE group_id = :group_id AND is_arranged = 1 AND due_date BETWEEN :start_date AND :end_date";
	query.prepare(sqlText);
	query.bindValue(":group_id", groupId);
	QDateTime startDate = QDateTime::currentDateTime();
	QDateTime endDate = startDate.addDays(6);
	query.bindValue(":start_date", startDate);
	query.bindValue(":end_date", endDate);

	bool ok = query.exec();
	if (!ok) {
		return QList<KTask>();
	}

	while (query.next()) {
		// ��ȡ��ѯ����е������ֶ�ֵ
		int id = query.value("id").toInt();
		int groupId = query.value("group_id").isNull() ? -1 : query.value("group_id").toInt();
		int userId = -1;
		QString eventName = query.value("event_name").toString();
		bool isArranged = query.value("is_arranged").toBool();
		int priority = query.value("priority").toInt();

		QDateTime dueDate = query.value("due_date").isNull() ? QDateTime() : query.value("due_date").toDateTime();
		bool isRepeated = query.value("is_repeated").toBool();
		QDateTime remindTime = query.value("remind_time").isNull() ? QDateTime() : query.value("remind_time").toDateTime();

		QString description = query.value("description").toString();
		QString location = query.value("location").toString();
		QString note = query.value("note").toString();


		KTask task(
			id, groupId, eventName, isArranged,
			priority, dueDate, isRepeated, remindTime, description, location, note, true
		);
		taskList.append(task);
	}

	return taskList;
}
// ��ѯ����7���
QList<KTask> KDatabase::selectTaskAfterNextWeekGroup(int groupId)
{
	if (!m_db.open()) {
		return QList<KTask>();
	}

	QList<KTask> taskList;
	QSqlQuery query;

	QString sqlText = "SELECT * FROM todo_list WHERE group_id = :group_id AND is_arranged = 1 AND due_date > :end_date";
	query.prepare(sqlText);
	query.bindValue(":group_id", groupId);
	QDateTime endDate = QDateTime::currentDateTime().addDays(7);
	query.bindValue(":end_date", endDate);

	bool ok = query.exec();
	if (!ok) {
		return QList<KTask>();
	}

	while (query.next()) {
		// ��ȡ��ѯ����е������ֶ�ֵ
		int id = query.value("id").toInt();
		int groupId = query.value("group_id").isNull() ? -1 : query.value("group_id").toInt();
		int userId = -1;
		QString eventName = query.value("event_name").toString();
		bool isArranged = query.value("is_arranged").toBool();
		int priority = query.value("priority").toInt();

		QDateTime dueDate = query.value("due_date").isNull() ? QDateTime() : query.value("due_date").toDateTime();
		bool isRepeated = query.value("is_repeated").toBool();
		QDateTime remindTime = query.value("remind_time").isNull() ? QDateTime() : query.value("remind_time").toDateTime();

		QString description = query.value("description").toString();
		QString location = query.value("location").toString();
		QString note = query.value("note").toString();


		KTask task(
			id, groupId, eventName, isArranged,
			priority, dueDate, isRepeated, remindTime, description, location, note, true
		);
		taskList.append(task);
	}

	return taskList;
}
// ��ѯ���������
QList<KTask> KDatabase::selectHaveFinishedTasksGroup(int groupId)
{
	if (!m_db.open()) {
		return QList<KTask>();
	}

	QList<KTask> taskList;
	QSqlQuery query;

	QString sqlText = "SELECT * FROM todo_list WHERE group_id = :group_id AND is_arranged = 1 AND due_date <= :end_date";
	query.prepare(sqlText);
	query.bindValue(":group_id", groupId);
	QDateTime endDate = QDateTime::currentDateTime();
	query.bindValue(":end_date", endDate);

	bool ok = query.exec();
	if (!ok) {
		return QList<KTask>();
	}

	while (query.next()) {
		// ��ȡ��ѯ����е������ֶ�ֵ
		int id = query.value("id").toInt();
		int groupId = query.value("group_id").isNull() ? -1 : query.value("group_id").toInt();
		int userId = -1;
		QString eventName = query.value("event_name").toString();
		bool isArranged = query.value("is_arranged").toBool();
		int priority = query.value("priority").toInt();

		QDateTime dueDate = query.value("due_date").isNull() ? QDateTime() : query.value("due_date").toDateTime();
		bool isRepeated = query.value("is_repeated").toBool();
		QDateTime remindTime = query.value("remind_time").isNull() ? QDateTime() : query.value("remind_time").toDateTime();

		QString description = query.value("description").toString();
		QString location = query.value("location").toString();
		QString note = query.value("note").toString();


		KTask task(
			id, groupId, eventName, isArranged,
			priority, dueDate, isRepeated, remindTime, description, location, note, true
		);
		taskList.append(task);
	}

	return taskList;
}
// ��ѯ�������д���
QList<KTask> KDatabase::selectAllTasksGroup(int groupId)
{
	if (!m_db.open()) {
		return QList<KTask>();
	}

	QList<KTask> taskList;
	QSqlQuery query;

	QString sqlText = "SELECT * FROM todo_list WHERE group_id = :group_id";
	query.prepare(sqlText);
	query.bindValue(":group_id", groupId);

	bool ok = query.exec();
	if (!ok) {
		return QList<KTask>();
	}

	while (query.next()) {
		// ��ȡ��ѯ����е������ֶ�ֵ
		int id = query.value("id").toInt();
		int groupId = query.value("group_id").isNull() ? -1 : query.value("group_id").toInt();
		int userId = -1;
		QString eventName = query.value("event_name").toString();
		bool isArranged = query.value("is_arranged").toBool();
		int priority = query.value("priority").toInt();

		QDateTime dueDate = query.value("due_date").isNull() ? QDateTime() : query.value("due_date").toDateTime();
		bool isRepeated = query.value("is_repeated").toBool();
		QDateTime remindTime = query.value("remind_time").isNull() ? QDateTime() : query.value("remind_time").toDateTime();

		QString description = query.value("description").toString();
		QString location = query.value("location").toString();
		QString note = query.value("note").toString();


		KTask task(
			id, groupId, eventName, isArranged,
			priority, dueDate, isRepeated, remindTime, description, location, note, true
		);
		taskList.append(task);
	}

	return taskList;
}
// ��ѯ�û�������Ϣ
QList<int> KDatabase::selectUserInfoByUserId(int user_id)
{
	QList<int>list;
	if (!m_db.open()) {
		return list;
	}

	QList<KTask> taskList;
	QSqlQuery query;

	QString sqlText = "SELECT group_id FROM todo_info WHERE user_id = :user_id";
	query.prepare(sqlText);
	query.bindValue(":user_id", user_id);

	bool ok = query.exec();
	if (!ok) {
		return list;
	}
	while (query.next()) {
		// ��ȡ��ѯ����е������ֶ�ֵ
		list.append(query.value(0).toInt());
	}
	return list;
}
// ��ѯ������
QString KDatabase::selectGroupNameById(int id)
{
	if (!m_db.open()) {
		return "";
	}
	QSqlQuery query;
	QString sqlText = "SELECT group_name FROM todo_groups WHERE id = :group_id";
	query.prepare(sqlText);
	query.bindValue(":group_id", id);
	bool ok = query.exec();
	if (!ok) {
		return "";
	}

	if (query.next()) {
		return query.value(0).toString();
	}
	return "";
}
// ��ѯ���Ա
QString KDatabase::selectGroupsMemberIds(int group_id)
{
	if (!m_db.open()) {
		return "";
	}
	QSqlQuery query;
	QString sqlText = "SELECT member_ids FROM todo_groups WHERE id = :group_id";
	query.prepare(sqlText);
	query.bindValue(":group_id", group_id);
	bool ok = query.exec();
	if (!ok) {
		return "";
	}
	if (query.next()) {
		return query.value(0).toString();
	}
	return "";
}
// ��ѯ�û�ID
int KDatabase::selectUserID(QString username)
{
	if (!m_db.open()) {
		return -1;
	}

	QSqlQuery query;
	QString sqltext = "SELECT id FROM todo_user WHERE username = :username";
	query.prepare(sqltext);
	// ʹ��bindValue()�����������󶨵���ѯ���
	query.bindValue(":username", username);

	bool ok = query.exec();
	if (!ok) {
		return -1;
	}

	if (query.next()) {
		// �ӽ���л�ȡidֵ��������
		int userId = query.value(0).toInt();
		return userId;
	}
	else {
		return -1;
	}
}
// ��������
void KDatabase::transaction()
{
	m_db.transaction();
}
// �ύ����
void KDatabase::commit()
{
	m_db.commit();
}
// ����ع�
void KDatabase::rollback()
{
	m_db.rollback();
}
