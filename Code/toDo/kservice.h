#ifndef KSERVICE_H
#define KSERVICE_H

#include <QObject>
#include "kdatabase.h"
#include "ktask.h"
/**
 * 业务层，业务逻辑放在Service中，更适合用来处理复杂的业务逻辑，方便扩展和维护，项目解耦合
 */
class KService  : public QObject
{
	Q_OBJECT

public:
	KService(QObject *parent);
	~KService();
	QString m_daoMsg;
	bool login(QString username, QString password);		// 登录业务
	bool regist(QString username, QString password);		// 注册业务
	bool deleteTaskByID(int id);			// 删除待办任务
	bool deleteUsersGroup(int group_id, int user_id, QString username);		// 删除用户所属分组
	bool createNewTaskByName(QString taskName, int userId);		// 新建待办任务
	bool createNewTaskByGroupId(int group_id, QString task_name);		// 新建待办任务
	bool createInfo(QString username, int group_id);		// 新建邀请消息
	int selectUserID(QString username);		//	查询用户ID
	bool updateDueDate(int task_id, QDateTime due_date);		// 更新截止时间
	bool taskNameChange(int task_id, QString new_name);		// 更改待办任务名字
	bool updateRemindTime(int task_id, QDateTime remindTime, bool is_repeated);		// 更新待办提醒时间
	bool updateDescription(int task_id, QString description);		// 更新待办描述
	bool updateLocation(int task_id, QString location);		// 更新待办地点
	bool updateNote(int task_id, QString note);		// 更新备注
	bool updatePriority(int task_id, int index);		// 更新优先级
	bool updateGroupName(int group_id, QString newName);		// 更新分组名字
	bool acceptInfo(int group_id, int user_id, QString username);		// 接受分组邀请
	bool refuseInfo(int group_id, int user_id);		// 拒绝分组邀请
	bool createNewGroup(QString groupName, int user_id, QString username);		// 新建分组
	QMap<int, QString>selectGroup(QString username);		// 查询分组
	QMap<int, QString>selectUserInfo(int user_id);		// 查询用户收到的邀请
	QList<KTask>selectUnscheduledTasks(QString username);		// 查询未安排任务 废弃函数
	QList<KTask>selectNext7DaysTasks(QString username);		// 查询近7天安排任务 废弃函数
	QList<KTask>selectAfterNextWeekTasks(QString username);		// 查询一周后安排任务 废弃函数
	QList<KTask>selectHaveFinishedTasks(QString username);		// 查询已经完成的任务 废弃函数
	QList<KTask>selectAllTasks(QString username);		// 查询所有任务
	QString selectGroupNameById(int id);		// 查询分组名字
private:
	void initDao();		// DAO层初始化
	void initConnect();		// 初始化信号和槽函数
	KDatabase* m_dao;		// DAO对象
};
#endif // KSERVICE_H