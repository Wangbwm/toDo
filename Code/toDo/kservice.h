#ifndef KSERVICE_H
#define KSERVICE_H

#include <QObject>
#include "kdatabase.h"
#include "ktask.h"
/**
 * ҵ��㣬ҵ���߼�����Service�У����ʺ����������ӵ�ҵ���߼���������չ��ά������Ŀ�����
 */
class KService  : public QObject
{
	Q_OBJECT

public:
	KService(QObject *parent);
	~KService();
	QString m_daoMsg;
	bool login(QString username, QString password);		// ��¼ҵ��
	bool regist(QString username, QString password);		// ע��ҵ��
	bool deleteTaskByID(int id);			// ɾ����������
	bool deleteUsersGroup(int group_id, int user_id, QString username);		// ɾ���û���������
	bool createNewTaskByName(QString taskName, int userId);		// �½���������
	bool createNewTaskByGroupId(int group_id, QString task_name);		// �½���������
	bool createInfo(QString username, int group_id);		// �½�������Ϣ
	int selectUserID(QString username);		//	��ѯ�û�ID
	bool updateDueDate(int task_id, QDateTime due_date);		// ���½�ֹʱ��
	bool taskNameChange(int task_id, QString new_name);		// ���Ĵ�����������
	bool updateRemindTime(int task_id, QDateTime remindTime, bool is_repeated);		// ���´�������ʱ��
	bool updateDescription(int task_id, QString description);		// ���´�������
	bool updateLocation(int task_id, QString location);		// ���´���ص�
	bool updateNote(int task_id, QString note);		// ���±�ע
	bool updatePriority(int task_id, int index);		// �������ȼ�
	bool updateGroupName(int group_id, QString newName);		// ���·�������
	bool acceptInfo(int group_id, int user_id, QString username);		// ���ܷ�������
	bool refuseInfo(int group_id, int user_id);		// �ܾ���������
	bool createNewGroup(QString groupName, int user_id, QString username);		// �½�����
	QMap<int, QString>selectGroup(QString username);		// ��ѯ����
	QMap<int, QString>selectUserInfo(int user_id);		// ��ѯ�û��յ�������
	QList<KTask>selectUnscheduledTasks(QString username);		// ��ѯδ�������� ��������
	QList<KTask>selectNext7DaysTasks(QString username);		// ��ѯ��7�찲������ ��������
	QList<KTask>selectAfterNextWeekTasks(QString username);		// ��ѯһ�ܺ������� ��������
	QList<KTask>selectHaveFinishedTasks(QString username);		// ��ѯ�Ѿ���ɵ����� ��������
	QList<KTask>selectAllTasks(QString username);		// ��ѯ��������
	QString selectGroupNameById(int id);		// ��ѯ��������
private:
	void initDao();		// DAO���ʼ��
	void initConnect();		// ��ʼ���źźͲۺ���
	KDatabase* m_dao;		// DAO����
};
#endif // KSERVICE_H