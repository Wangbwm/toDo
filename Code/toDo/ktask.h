#ifndef KTASK_H
#define KTASK_H
#include <QVariant>
#include <QMetaType>
#include <QString>
#include <QTime>
#include <QDate>
/**
 * 待办任务对象类
 */
class KTask
{
	Q_GADGET
public:
	KTask();
	KTask(int eventId, int userId, const QString& eventName, bool isArranged, int priority,
		const QDateTime& dueDate, bool isRepeated, const QDateTime& remindTime,
		const QString& description, const QString& location, const QString& note);
	KTask(int eventId, int groupId, const QString& eventName, bool isArranged, int priority,
		const QDateTime& dueDate, bool isRepeated, const QDateTime& remindTime,
		const QString& description, const QString& location, const QString& note, bool group);
	~KTask();
	int getId() const;
	int getUserId() const;
	int getGroupId() const;
	QString getEventName() const;
	bool isArranged() const;
	int getPriority() const;
	QDateTime getDueDate() const;
	bool isRepeated() const;
	QDateTime getRemindTime() const;
	QString getDescription() const;
	QString getLocation() const;
	QString getNote() const;
private:
	int m_id;		// id
	int userId;		// 用户ID
	int groupId;		// 分组ID
	QString eventName;		// 待办名字
	bool arranged;		// 是否安排	
	int priority;		// 优先级
	QDateTime dueDate;		// 截止日期
	bool m_repeated;		// 是否重复
	QDateTime remindTime;		// 提醒日期
	QString description;		// 描述
	QString location;		// 地点
	QString note;		// 备注
};
Q_DECLARE_METATYPE(KTask)		// 注册元对象
#endif // KTASK_H