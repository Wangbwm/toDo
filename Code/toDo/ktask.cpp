#include "ktask.h"
KTask::KTask()
	: m_id(-1)
	, userId(-1)
	, groupId(-1)
	, eventName("")
	, arranged(false)
	, priority(0)
	, dueDate(QDateTime())
	, m_repeated(false)
	, remindTime(QDateTime())
	, description("")
	, location("")
	, note("")
{}
KTask::KTask(int eventId, int userId, const QString& eventName, bool isArranged, int priority,
	const QDateTime& dueDate, bool isRepeated, const QDateTime& remindTime,
	const QString& description, const QString& location, const QString& note)
	: m_id(eventId)
	, userId(userId)
	, groupId(-1)
	, eventName(eventName)
	, arranged(isArranged)
	, priority(priority)
	, dueDate(dueDate)
	, m_repeated(isRepeated)
	, remindTime(remindTime)
	, description(description)
	, location(location)
	, note(note)
{
	
}

KTask::KTask(int eventId, int groupId, const QString& eventName, bool isArranged, int priority,
	const QDateTime& dueDate, bool isRepeated, const QDateTime& remindTime,
	const QString& description, const QString& location, const QString& note, bool group)
	: m_id(eventId)
	, userId(-1)
	, groupId(groupId)
	, eventName(eventName)
	, arranged(isArranged)
	, priority(priority)
	, dueDate(dueDate)
	, m_repeated(isRepeated)
	, remindTime(remindTime)
	, description(description)
	, location(location)
	, note(note)
{
	
}



KTask::~KTask()
{}

int KTask::getId() const
{
	return m_id;
}

int KTask::getUserId() const
{
	return userId;
}

int KTask::getGroupId() const
{
	return groupId;
}

QString KTask::getEventName() const
{
	return eventName;
}

bool KTask::isArranged() const
{
	return arranged;
}

int KTask::getPriority() const
{
	return priority;
}

QDateTime KTask::getDueDate() const
{
	return dueDate;
}

bool KTask::isRepeated() const
{
	return m_repeated;
}

QDateTime KTask::getRemindTime() const
{
	return remindTime;
}

QString KTask::getDescription() const
{
	return description;
}

QString KTask::getLocation() const
{
	return location;
}

QString KTask::getNote() const
{
	return note;
}
