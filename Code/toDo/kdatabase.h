#ifndef KDATABASE_H
#define KDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QEventLoop>
#include "ktask.h"
/*
* DAO��(���ݷ��ʲ�),���������ݿ���������һЩ���񶼷�װ�ڴ�
*/
class KDatabase  : public QObject
{
	Q_OBJECT

public:
	KDatabase(QObject *parent);
	~KDatabase();
	QString m_daoMsg;
	bool selectAccount(QString username, QString password);		// ��ѯ�˻�
	bool selectAccountByName(QString username);		// �����û�����ѯ�˻�
	bool selectUserInfoByUserIdAndGroupId(int user_id, int group_id);		// �����û�ID����ID��ѯ������Ϣ
	bool selectHaveGroup(int group_id);				// ��ѯ���Ƿ����
	bool insertAccount(QString username, QString password);		// �½��˺ű��û�
	bool insertTodoUser(QString username);		// �½��û����û�
	bool insertGroup(QString groupName, int user_id);		// �½�����
	bool insertInfo(int user_id, int group_id);		// �½�������Ϣ
	bool deleteTaskByID(int id);		// �����û�IDɾ������
	bool deleteGroupByID(int id);		// ����IDɾ����
	bool deleteTaskByGroupId(int group_id);		// ������IDɾ������
	bool deleteInfo(int group_id, int user_id);		// ɾ��������Ϣ
	bool createNewTaskByName(QString taskName, int userId);		// �����û�ID�½�����
	bool createNewTaskByGroupId(int group_id, QString taskName);		// ���ݷ���ID�½�����
	bool updateDueDate(int task_id, QDateTime due_date);		// ���½�ֹ����
	bool updateTaskName(int task_id, QString new_name);		// ���´�������
	bool updateRemindTime(int task_id, QDateTime remindTime, bool is_repeated);		// ��������ʱ��
	bool updateDescription(int task_id, QString description);		// ��������
	bool updateLocation(int task_id, QString location);		// ���µص�
	bool updateNote(int task_id, QString note);		// ���±�ע
	bool updatePriority(int task_id, int index);		// �������ȼ�
	bool updateUserGroup(int user_id, QString group_list);		// �����û��������б�
	bool updateGroupMember(int group_id, QString mem_list);		// ���·����û��б�
	bool updateGroupNameById(int group_id, QString newName);		// ���·�������
	QVariant selectUsersGroup(QString username);		// ��ѯ�û��������б�
	QMap<int, QString>selectGroupID(QString groupName);		// ���ݷ������ֲ�ѯ����Ӧ�ı��id��������

	QList<KTask>selectTaskUnscheduled(int userId);		// ��ѯδ�������� ������������ѯ���к���c++�������Ч�ʸ���
	QList<KTask>selectTaskNext7Days(int userId);		// ��ѯ��7������ ����������ͬ��
	QList<KTask>selectTaskNextWeek(int userId);		// ��ѯ7��֮������� ����������ͬ��
	QList<KTask>selectHaveFinishedTasks(int userId);	// ��ѯ�Ѿ���ɵ����� ��������
	QList<KTask>selectAllTasks(int userId);		// ��ѯ��������

	QList<KTask>selectTaskUnscheduledGroup(int groupId);		// ��ѯ���鰲������ ��������
	QList<KTask>selectTaskNext7DaysGroup(int groupId);		// ��ѯ�����7������ ��������
	QList<KTask>selectTaskAfterNextWeekGroup(int groupId);		// ��ѯ����7��֮������� ��������
	QList<KTask>selectHaveFinishedTasksGroup(int groupId);		// ��ѯ�����Ѿ���ɵ����� ��������
	QList<KTask>selectAllTasksGroup(int groupId);		// ��ѯ������������

	QList<int>selectUserInfoByUserId(int user_id);		// ��ѯ�û��յ���������Ϣ������������ID��List����

	QString selectGroupNameById(int id);		// ��ѯ������
	QString selectGroupsMemberIds(int group_id);		// ��ѯ�������İ����û�id

	int selectUserID(QString username);		// ��ѯ�û�ID
	void transaction();		// �����������
	void commit();		// �ύ�������
	void rollback();		// �ع�����
private:
	void Server_conn();		// �������ݿ�
	QSqlDatabase m_db;		// ����Դ����
};
#endif // KDATABASE_H