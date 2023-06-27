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
* DAO层(数据访问层),负责与数据库进行联络的一些任务都封装在此
*/
class KDatabase  : public QObject
{
	Q_OBJECT

public:
	KDatabase(QObject *parent);
	~KDatabase();
	QString m_daoMsg;
	bool selectAccount(QString username, QString password);		// 查询账户
	bool selectAccountByName(QString username);		// 根据用户名查询账户
	bool selectUserInfoByUserIdAndGroupId(int user_id, int group_id);		// 根据用户ID和组ID查询邀请消息
	bool selectHaveGroup(int group_id);				// 查询组是否存在
	bool insertAccount(QString username, QString password);		// 新建账号表用户
	bool insertTodoUser(QString username);		// 新建用户表用户
	bool insertGroup(QString groupName, int user_id);		// 新建分组
	bool insertInfo(int user_id, int group_id);		// 新建邀请信息
	bool deleteTaskByID(int id);		// 根据用户ID删除待办
	bool deleteGroupByID(int id);		// 根据ID删除组
	bool deleteTaskByGroupId(int group_id);		// 根据组ID删除待办
	bool deleteInfo(int group_id, int user_id);		// 删除邀请消息
	bool createNewTaskByName(QString taskName, int userId);		// 根据用户ID新建待办
	bool createNewTaskByGroupId(int group_id, QString taskName);		// 根据分组ID新建待办
	bool updateDueDate(int task_id, QDateTime due_date);		// 更新截止日期
	bool updateTaskName(int task_id, QString new_name);		// 更新待办名字
	bool updateRemindTime(int task_id, QDateTime remindTime, bool is_repeated);		// 更新提醒时间
	bool updateDescription(int task_id, QString description);		// 更新描述
	bool updateLocation(int task_id, QString location);		// 更新地点
	bool updateNote(int task_id, QString note);		// 更新备注
	bool updatePriority(int task_id, int index);		// 更新优先级
	bool updateUserGroup(int user_id, QString group_list);		// 更新用户所在组列表
	bool updateGroupMember(int group_id, QString mem_list);		// 更新分组用户列表
	bool updateGroupNameById(int group_id, QString newName);		// 更新分组名字
	QVariant selectUsersGroup(QString username);		// 查询用户所在组列表
	QMap<int, QString>selectGroupID(QString groupName);		// 根据分组名字查询到对应的表项：id，分组名

	QList<KTask>selectTaskUnscheduled(int userId);		// 查询未安排任务 废弃函数，查询所有后在c++里面分类效率更高
	QList<KTask>selectTaskNext7Days(int userId);		// 查询近7天任务 废弃函数，同上
	QList<KTask>selectTaskNextWeek(int userId);		// 查询7天之后的任务 废弃函数，同上
	QList<KTask>selectHaveFinishedTasks(int userId);	// 查询已经完成的任务 废弃函数
	QList<KTask>selectAllTasks(int userId);		// 查询所有任务

	QList<KTask>selectTaskUnscheduledGroup(int groupId);		// 查询分组安排任务 废弃函数
	QList<KTask>selectTaskNext7DaysGroup(int groupId);		// 查询分组近7天任务 废弃函数
	QList<KTask>selectTaskAfterNextWeekGroup(int groupId);		// 查询分组7天之后的任务 废弃函数
	QList<KTask>selectHaveFinishedTasksGroup(int groupId);		// 查询分组已经完成的任务 废弃函数
	QList<KTask>selectAllTasksGroup(int groupId);		// 查询分组所有任务

	QList<int>selectUserInfoByUserId(int user_id);		// 查询用户收到的邀请消息，返回邀请组ID的List容器

	QString selectGroupNameById(int id);		// 查询分组名
	QString selectGroupsMemberIds(int group_id);		// 查询分组表项的包含用户id

	int selectUserID(QString username);		// 查询用户ID
	void transaction();		// 开启事务管理
	void commit();		// 提交事务操作
	void rollback();		// 回滚事务
private:
	void Server_conn();		// 连接数据库
	QSqlDatabase m_db;		// 数据源对象
};
#endif // KDATABASE_H