#ifndef KTASK_H
#define KTASK_H
#include <QVariant>
#include <QMetaType>
#include <QString>
#include <QTime>
#include <QDate>
/**
 * �������������
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
	int userId;		// �û�ID
	int groupId;		// ����ID
	QString eventName;		// ��������
	bool arranged;		// �Ƿ���	
	int priority;		// ���ȼ�
	QDateTime dueDate;		// ��ֹ����
	bool m_repeated;		// �Ƿ��ظ�
	QDateTime remindTime;		// ��������
	QString description;		// ����
	QString location;		// �ص�
	QString note;		// ��ע
};
Q_DECLARE_METATYPE(KTask)		// ע��Ԫ����
#endif // KTASK_H