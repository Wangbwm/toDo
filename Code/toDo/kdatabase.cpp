#include "kdatabase.h"
/**
 * 数据访问层具体实现，连接数据库实现增删改查工作
 */
KDatabase::KDatabase(QObject *parent)
	: QObject(parent)
{
	Server_conn();
}
// 关闭数据库连接
KDatabase::~KDatabase()
{
	m_db.close();
}

// 初始化数据源
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
	m_db.setHostName("***********"); //
	m_db.setPort(3306);       // 如果使用的是默认端口可以不设置
	m_db.setUserName("*******");	// 数据库用户名
	m_db.setPassword("********"); // 数据库密码
	m_db.setDatabaseName("todo"); // 数据库名字
	if (m_db.open())
	{
		qDebug() << QStringLiteral("数据库打开成功, 可以读写数据了......");
		m_daoMsg = QStringLiteral("数据库打开成功, 可以读写数据了......");
	}
	else
	{
		qDebug() << QStringLiteral("数据库打开失败: ") + m_db.lastError().text();
		m_daoMsg = QStringLiteral("数据库打开失败: ") + m_db.lastError().text();
	}
}

// 根据用户名密码查询用户，使用prepare将sql语句格式定下来，防止sql注入
bool KDatabase::selectAccount(QString username, QString password)
{
	if (!m_db.open()) {
		return false;
	}
	QSqlQuery query;
	//防止sql注入
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
		// 查询结果为空
		return false;
	}
	else {
		// 查询结果非空
		return true;
	}
}
// 根据用户名查询用户，使用prepare将sql语句格式定下来，防止sql注入
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
		// 查询结果为空
		return false;
	}
	else {
		// 查询结果非空
		return true;
	}
	return false;
}

// 查询用户收到邀请消息
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
		// 查询结果为空
		return false;
	}
	else {
		// 查询结果非空
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
		// 查询结果为空
		return false;
	}
	else {
		// 查询结果非空
		return true;
	}
	return false;
}
// 新建用户
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
// 新建用户表用户
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
// 新建分组
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
// 新建邀请信息
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
// 删除待办
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
// 删除分组
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
// 删除待办
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
// 删除消息
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

// 新建待办
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
// 新建待办
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
// 更新截止日期
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
// 更新待办名字
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
// 更新提醒时间
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
// 更新描述
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
// 更新地点
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
// 更新备注
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
// 更新优先级
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
// 更新用户分组
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
// 更新分组用户
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
// 更新分组名字
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


// 查询用户分组
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
		// 获取查询结果并绑定到QVariant实例上
		QVariant groupsList = query.value(0);
		return groupsList;
	}
	else {
		return QVariant();
	}
}
// 查询分组ID
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
		// 获取查询结果中的所有字段值
		int id = query.value("id").toInt();
		QString member_ids = query.value("member_ids").toString();
		map.insert(id, member_ids);
	}
	return map;
}
// 查询未安排
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
		// 获取查询结果中的所有字段值
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
// 查询近七天
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
		// 获取查询结果中的所有字段值
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
// 查询七天后
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
		// 获取查询结果中的所有字段值
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
// 查询已完成
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
		// 获取查询结果中的所有字段值
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
// 查询所有待办
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
		// 获取查询结果中的所有字段值
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
// 查询分组未完成
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
		// 获取查询结果中的所有字段值
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
// 查询分组7天内
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
		// 获取查询结果中的所有字段值
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
// 查询分组7天后
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
		// 获取查询结果中的所有字段值
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
// 查询分组已完成
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
		// 获取查询结果中的所有字段值
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
// 查询分组所有待办
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
		// 获取查询结果中的所有字段值
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
// 查询用户邀请消息
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
		// 获取查询结果中的所有字段值
		list.append(query.value(0).toInt());
	}
	return list;
}
// 查询组名字
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
// 查询组成员
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
// 查询用户ID
int KDatabase::selectUserID(QString username)
{
	if (!m_db.open()) {
		return -1;
	}

	QSqlQuery query;
	QString sqltext = "SELECT id FROM todo_user WHERE username = :username";
	query.prepare(sqltext);
	// 使用bindValue()方法将参数绑定到查询语句
	query.bindValue(":username", username);

	bool ok = query.exec();
	if (!ok) {
		return -1;
	}

	if (query.next()) {
		// 从结果中获取id值并返回它
		int userId = query.value(0).toInt();
		return userId;
	}
	else {
		return -1;
	}
}
// 开启事务
void KDatabase::transaction()
{
	m_db.transaction();
}
// 提交事务
void KDatabase::commit()
{
	m_db.commit();
}
// 事务回滚
void KDatabase::rollback()
{
	m_db.rollback();
}
