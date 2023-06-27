#include "kmainwindow.h"
#include <QAbstractItemView>
#include <QRadioButton>
#include <QTreeWidgetItem>
#include <QDateTimeEdit>
#include <QTextEdit>
#include "kthread.h"
KMainWindow::KMainWindow(QWidget *parent)
    : QWidget(parent)
    , m_loginWindow(nullptr)
    , m_dialog(nullptr)
    , m_service(nullptr)
	, m_unscheduledTasks(nullptr)
	, m_next7DaysTasks(nullptr)
	, m_afterNextWeekTasks(nullptr)
	, m_AllTasks(nullptr)
	, m_HaveFinishedTasks(nullptr)
	, m_usersGroup(nullptr)
	, m_infoMap(nullptr)
{
    ui.setupUi(this);
    init();
    initConnect();
}

KMainWindow::~KMainWindow()
{
    delete m_loginWindow;
	if (m_unscheduledTasks != nullptr)m_unscheduledTasks->clear();
	delete m_unscheduledTasks;
	if (m_next7DaysTasks != nullptr)m_next7DaysTasks->clear();
	delete m_next7DaysTasks;
	if (m_afterNextWeekTasks != nullptr)m_afterNextWeekTasks->clear();
	delete m_afterNextWeekTasks;
	if (m_HaveFinishedTasks != nullptr)m_HaveFinishedTasks->clear();
	delete m_HaveFinishedTasks;
	if (m_AllTasks != nullptr)m_AllTasks->clear();
	delete m_AllTasks;
	if (m_usersGroup != nullptr)m_usersGroup->clear();
	delete m_usersGroup;
	if (m_infoMap != nullptr)m_infoMap->clear();
	delete m_infoMap;
}
// 初始化
void KMainWindow::init()
{
	setWindowTitle(QStringLiteral("-待办-"));
	ui.dateTimeEditHome->setDateTime(QDateTime().currentDateTime());
	ui.dateTimeEditHome_2->setDateTime(QDateTime().currentDateTime());

    m_loginWindow = new KLoginWindow;
	m_loginWindow->show();		// 展示登录窗口


	ui.treeWidget->header()->setVisible(false);
	ui.treeWidgetGroup->header()->setVisible(false);
	ui.treeWidgetRight->header()->setVisible(false);
	ui.treeWidgetLeft->header()->setVisible(false);


	// ui.lineEditName->setDisabled(true);
	
	ui.comboBoxMySort->setVisible(false);
	ui.stackedWidgetDueDate->setCurrentIndex(0);
	ui.stackedWidgetGroupAll->setCurrentIndex(0);
	ui.stackedWidgetGroupNew->setCurrentIndex(0);
	ui.stackedWidgetGroupNewTodo->setCurrentIndex(0);
	ui.stackedWidgetHomeNewtodo->setCurrentIndex(0);
	ui.stackedWidgetMid->setCurrentIndex(0);
	ui.stackedWidgetRemind->setCurrentIndex(0);
	ui.stackedWidgetRight->setCurrentIndex(0);
	ui.stackedWidgetRightMain->setCurrentIndex(0);
	ui.stackedWidgetSend->setCurrentIndex(0);


}
// 初始化主页视图
void KMainWindow::initHomeListWidget()
{
	ui.treeWidget->clear();
	// 创建三个根节点
	QTreeWidgetItem* unscheduledRoot = new QTreeWidgetItem(ui.treeWidget);
	unscheduledRoot->setText(0, QStringLiteral("未安排"));

	QTreeWidgetItem* next7DaysRoot = new QTreeWidgetItem(ui.treeWidget);
	next7DaysRoot->setText(0, QStringLiteral("未来七天"));

	QTreeWidgetItem* afterNextWeekRoot = new QTreeWidgetItem(ui.treeWidget);
	afterNextWeekRoot->setText(0, QStringLiteral("以后"));

	// 添加子项到不同的根节点中
	for (const auto task : *m_unscheduledTasks) {
		QTreeWidgetItem* taskItem = new QTreeWidgetItem();
		QString taskMes;
		taskMes = task.getEventName();
		int groupId = task.getGroupId();
		QString groupStr = checkGroup(groupId);
		taskMes += QStringLiteral(" 分组:") + groupStr;
		int priority = task.getPriority();
		QString priorityStr = checkPriority(priority);
		
		taskMes += QStringLiteral(" 优先级:") + priorityStr;
		taskItem->setText(0, taskMes);
		taskItem->setData(0, Qt::UserRole, QVariant(task.getId()));
		unscheduledRoot->addChild(taskItem);
	}

	// 添加子项到不同的根节点中
	for (const auto task : *m_next7DaysTasks) {
		QTreeWidgetItem* taskItem = new QTreeWidgetItem();
		QString taskMes;
		taskMes = task.getEventName();
		int groupId = task.getGroupId();
		QString groupStr = checkGroup(groupId);
		taskMes += QStringLiteral(" 分组:") + groupStr;
		int priority = task.getPriority();
		QString priorityStr = checkPriority(priority);

		taskMes += QStringLiteral(" 优先级:") + priorityStr;
		taskItem->setText(0, taskMes);
		taskItem->setData(0, Qt::UserRole, QVariant(task.getId()));
		next7DaysRoot->addChild(taskItem);
	}

	// 添加子项到不同的根节点中
	for (const auto task : *m_afterNextWeekTasks) {
		QTreeWidgetItem* taskItem = new QTreeWidgetItem();
		QString taskMes;
		taskMes = task.getEventName();
		int groupId = task.getGroupId();
		QString groupStr = checkGroup(groupId);
		taskMes += QStringLiteral(" 分组:") + groupStr;
		int priority = task.getPriority();
		QString priorityStr = checkPriority(priority);

		taskMes += QStringLiteral(" 优先级:") + priorityStr;
		taskItem->setText(0, taskMes);
		taskItem->setData(0, Qt::UserRole, QVariant(task.getId()));
		afterNextWeekRoot->addChild(taskItem);
	}
}
// 初始化分组页面视图
void KMainWindow::initGroupWidget()
{
	ui.treeWidgetGroup->clear();
	initLeftWidget();
}
// 初始化“我的待办”视图
void KMainWindow::initMyListWidget()
{
	ui.treeWidget->clear();
	QTreeWidgetItem* haveFinishedRoot = new QTreeWidgetItem(ui.treeWidget);
	haveFinishedRoot->setText(0, QStringLiteral("已完成"));


	QTreeWidgetItem* Root = new QTreeWidgetItem(ui.treeWidget);
	Root->setText(0, QStringLiteral("我的待办"));
	Root->setExpanded(true);

	if (ui.comboBoxMySort->currentIndex() == 0)		// 默认排序
	{
		
	}
	else if (ui.comboBoxMySort->currentIndex() == 1)		// 按到期时间排序
	{
		
		// 将任务按到期时间从早到晚排序
		std::sort(m_AllTasks->begin(), m_AllTasks->end(), [](const KTask& a, const KTask& b) {
			return a.getDueDate() < b.getDueDate();
			});

	}
	else if (ui.comboBoxMySort->currentIndex() == 2)		// 按创建时间排序
	{
		// 将任务按创建时间从早到晚排序
		std::sort(m_AllTasks->begin(), m_AllTasks->end(), [](const KTask& a, const KTask& b) {
			return a.getId() < b.getId();
			});
	}
	else               // 按优先级排序
	{
		// 将任务按优先级排序
		std::sort(m_AllTasks->begin(), m_AllTasks->end(), [](const KTask& a, const KTask& b) {
			return a.getPriority() > b.getPriority();
			});
	}
	// 添加子项
	for (const auto task : *m_AllTasks) {
		
		QTreeWidgetItem* taskItem = new QTreeWidgetItem();
		QString taskMes;
		taskMes = task.getEventName();
		int groupId = task.getGroupId();
		QString groupStr = checkGroup(groupId);
		taskMes += QStringLiteral(" 分组:") + groupStr;
		int priority = task.getPriority();
		QString priorityStr = checkPriority(priority);

		taskMes += QStringLiteral(" 优先级:") + priorityStr;
		taskItem->setText(0, taskMes);
		taskItem->setData(0, Qt::UserRole, QVariant(task.getId()));
		if (task.getDueDate().isValid()
			&& task.getDueDate() < QDateTime::currentDateTime())
		{
			haveFinishedRoot->addChild(taskItem);
		}
		else Root->addChild(taskItem);
		
	}
}
// 初始化左侧分组视图
void KMainWindow::initLeftWidget()
{
	ui.treeWidgetLeft->clear();
	m_usersGroup->clear();
	QTreeWidgetItem* Root = new QTreeWidgetItem(ui.treeWidgetLeft);
	Root->setText(0, QStringLiteral("我的组"));
	Root->setExpanded(true);
	m_usersGroup = new QMap<int, QString>(m_service->selectGroup(m_username));
	for (auto iter = m_usersGroup->constBegin(); iter != m_usersGroup->constEnd(); ++iter)
	{
		int group_id = iter.key();
		QString groupName = iter.value();
		QTreeWidgetItem* groupItem = new QTreeWidgetItem();
		groupItem->setText(0, groupName);
		groupItem->setData(0, Qt::UserRole, QVariant(group_id));
		Root->addChild(groupItem);
	}
}
// 初始化数据容器
void KMainWindow::initTaskList()
{
	setDisableRight();
	ui.stackedWidgetMid->setDisabled(true);
	KThread* worker = new KThread(m_username, this);
	connect(worker, &KThread::queryFinished, this, &KMainWindow::onAllTasksQueryFinished, Qt::QueuedConnection);

	// 查询所有任务数据
	worker->start();
}

//	检查是否有邀请信息
void KMainWindow::checkHaveInvite()
{
	m_infoMap->clear();
	m_infoMap = new QMap<int, QString>(m_service->selectUserInfo(m_userId));
	if (!m_infoMap->isEmpty()) // 说明有入组邀请了
	{
		bool check = false;
		for (auto it = m_usersGroup->begin(); it != m_usersGroup->end(); ++it)
		{
			if (it.key() == m_infoMap->firstKey())
			{
				check = true;
				break; // 找到对应元素后退出循环
			}
		}

		if (check) // 已经加入群组，从 map 中删除该元素
		{
			auto it = m_infoMap->begin(); // 返回第一个元素的迭代器
			m_infoMap->erase(it); // 删除第一个元素
		}
		else // 没有加入群组
		{
			QString msg = QStringLiteral("收到分组:") + m_infoMap->value(m_infoMap->firstKey()) + QStringLiteral(" 的邀请");
			ui.stackedWidgetGroupAll->setCurrentIndex(1);
			ui.textBrowserGetMsg->setText(msg);
			
		}
	}
}
// 右侧更改栏设置为不可选
void KMainWindow::setDisableRight()
{
	ui.stackedWidgetRight->setDisabled(true);
}
// 右侧更改栏设置为可选
void KMainWindow::setUnDisableRight()
{
	ui.stackedWidgetRight->setDisabled(false);
}


QString KMainWindow::checkGroup(int groupId)
{
	if (groupId == -1)
	{
		return QStringLiteral("我的待办");
	}
	else
	{
		return m_service->selectGroupNameById(groupId);
	}
}

QString KMainWindow::checkPriority(int priority)
{
	if (priority == 0)return QStringLiteral("无优先级");
	else if (priority == 1)return QStringLiteral("低");
	else if (priority == 2)return QStringLiteral("中");
	else return QStringLiteral("高");
}
// 初始化信号和槽函数
void KMainWindow::initConnect()
{
    (void)connect(m_loginWindow, &KLoginWindow::sendToMainWindow, this, [=](KDialogWindow* dialog, QString username) { //收到登录成功的信号后再初始化服务层、DAO层和视图以及模型等资源
        m_service = new KService(this);
        m_dialog = dialog;
        m_username = username;
		m_userId = m_service->selectUserID(m_username);
		m_unscheduledTasks = new QList<KTask>;
		m_next7DaysTasks = new QList<KTask>;
		m_afterNextWeekTasks = new QList<KTask>;
		m_AllTasks = new QList<KTask>;
		m_HaveFinishedTasks = new QList<KTask>;
		m_usersGroup = new QMap<int, QString>;
		m_infoMap = new QMap<int, QString>;
		ui.label_username->setText(m_username);
		ui.label_username->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		ui.btnHome->setChecked(true);
		//setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);	// 顶层运行
		initTaskList();
		initHomeListWidget();
		//initLeftWidget();
		this->show();
		initConnectHome();
		initConnectGroup();
		setDisableRight();
		(void)connect(ui.btnHome, &QPushButton::clicked, this, [=]() {
			ui.labelHome->setText(QStringLiteral("我的首页"));
			setDisableRight();
			ui.stackedWidgetMid->setCurrentIndex(0);
			ui.stackedWidgetRight->setCurrentIndex(0);
			ui.comboBoxMySort->setVisible(false);
			initHomeListWidget();
			});
		(void)connect(ui.btnTodo, &QPushButton::clicked, this, [=]() {
			ui.labelHome->setText(QStringLiteral("我的待办"));
			setDisableRight();
			ui.stackedWidgetMid->setCurrentIndex(0);
			ui.stackedWidgetRight->setCurrentIndex(0);
			ui.comboBoxMySort->setCurrentIndex(0);
			ui.comboBoxMySort->setVisible(true);
			initMyListWidget();
			});
		(void)connect(ui.btnGroup, &QPushButton::clicked, this, [=]() {
			setDisableRight();
			checkHaveInvite();
			ui.stackedWidgetMid->setCurrentIndex(1);
			ui.stackedWidgetRight->setCurrentIndex(0);
			ui.comboBoxGroupSort->setCurrentIndex(0);
			});
		
        });


}
// 初始化主页信号和槽函数
void KMainWindow::initConnectHome()
{
	(void)connect(ui.btnNewToDo, &QPushButton::clicked, this, &KMainWindow::btnNewToDoClicked);
	(void)connect(ui.btnHomeDate, &QPushButton::clicked, this, &KMainWindow::btnHomeDateClicked);
	(void)connect(ui.dateTimeEditHome, &QDateTimeEdit::editingFinished, this, &KMainWindow::changeHomeDueDate);
	(void)connect(ui.dateTimeEditHome_2, &QDateTimeEdit::editingFinished, this, &KMainWindow::changeHomeRemind_time);
	(void)connect(ui.lineEditName, &QLineEdit::returnPressed, this, &KMainWindow::lineEditNameChange);
	(void)connect(ui.btnHomeRemind, &QPushButton::clicked, this, &KMainWindow::btnHomeRemindClicked);
	(void)connect(ui.btnHomeLocation, &QPushButton::toggled, this, &KMainWindow::btnHomeLocationClicked);
	(void)connect(ui.btnHomeNote, &QPushButton::toggled, this, &KMainWindow::btnHomeNoteClicked);
	(void)connect(ui.btnHomeDescription, &QPushButton::toggled, this, &KMainWindow::btnHomeDescriptionClicked);
	(void)connect(ui.btnHomeOK, &QPushButton::clicked, this, &KMainWindow::btnHomeOKClicked);
	(void)connect(ui.comboBoxHome, QOverload<int>::of(&QComboBox::activated), this, &KMainWindow::comboBoxHomeChange);
	(void)connect(ui.comboBoxMySort, QOverload<int>::of(&QComboBox::activated), this, [=]() {
		initMyListWidget();
		});
	(void)connect(ui.btnHomeCloseBigger, &QPushButton::clicked, this, [=]() {
		ui.stackedWidgetRightMain->setCurrentIndex(0);
		});

	(void)connect(ui.lineEditNewTodo, &QLineEdit::returnPressed, this, [=]() {
		ui.stackedWidgetHomeNewtodo->setCurrentIndex(0);
		QString taskName = ui.lineEditNewTodo->text();
		ui.lineEditNewTodo->clear(); // 清空输入框内容
		if (m_service->createNewTaskByName(taskName, m_userId))
		{
			m_dialog->setMsg(QStringLiteral("添加成功"), QStringLiteral("您已经成功添加任务"));
			m_dialog->show();
			ui.btnRotate->click();
		}
		else
		{
			m_dialog->setMsg(QStringLiteral("添加失败"), QStringLiteral("请检查数据库连接"));
			m_dialog->show();
			ui.btnRotate->click();
		}
		

		});
	// 将 treeWidget 的 itemPressed 信号连接到 on_treeWidget_itemClicked 槽函数上
	(void)connect(ui.treeWidget, &QTreeWidget::itemPressed, this, &KMainWindow::on_treeWidget_itemClicked);
	(void)connect(ui.treeWidgetRight, &QTreeWidget::itemPressed, this, &KMainWindow::on_treeWidgetRight_itemClicked);

	(void)connect(ui.btnRotate, &QPushButton::clicked, this, &KMainWindow::btnRotateClicked);
	(void)connect(ui.checkBoxHome, &QCheckBox::clicked, this, [=]() {
		int task_id;
		if (ui.btnGroup->isChecked())
		{
			task_id = ui.treeWidgetGroup->currentItem()->data(0, Qt::UserRole).toUInt();
		}
		else
		{
			task_id = ui.treeWidget->currentItem()->data(0, Qt::UserRole).toUInt();
		}
		 
		if (task_id == 0)
		{
			ui.checkBoxHome->setChecked(false); return;
		}
		else
		{
			if (m_service->deleteTaskByID(task_id))
			{
				m_dialog->setMsg(QStringLiteral("删除成功"), QStringLiteral("您已经成功删除任务"));
				m_dialog->show();
				ui.checkBoxHome->setChecked(false);
				ui.btnRotate->click();
			}
			else
			{
				m_dialog->setMsg(QStringLiteral("删除失败"), QStringLiteral("请检查数据库连接"));
				m_dialog->show();
				ui.checkBoxHome->setChecked(false);
			}
		}
		});
}
// 初始化分组页面信号和槽函数
void KMainWindow::initConnectGroup()
{
	(void)connect(ui.treeWidgetLeft, &QTreeWidget::itemPressed, this, &KMainWindow::on_treeWidgetLeft_itemClicked);
	(void)connect(ui.treeWidgetGroup, &QTreeWidget::itemPressed, this, &KMainWindow::on_treeWidgetGroup_itemClicked);
	(void)connect(ui.btnRotateGroup, &QPushButton::clicked, this, &KMainWindow::btnRotateClicked);
	(void)connect(ui.btnAccept, &QPushButton::clicked, this, &KMainWindow::btnAcceptClicked);
	(void)connect(ui.btnRefuse, &QPushButton::clicked, this, &KMainWindow::btnRefuseClicked);
	(void)connect(ui.btnChangeGroupName, &QPushButton::clicked, this, [=]() {
		ui.stackedWidgetGroupNew->setCurrentIndex(2);
		ui.lineEditGroupName->setPlaceholderText(QStringLiteral("输入内容后，回车修改成功")); // 设置输入框的提示文本
		connect(ui.lineEditGroupName, &QLineEdit::textChanged, this, [=](const QString& text) {
			if (!text.isEmpty()) { // 如果输入框不为空，隐藏提示文本
				ui.lineEditGroupName->setPlaceholderText("");
			}
			else { // 如果输入框为空，显示提示文本 
				ui.lineEditGroupName->setPlaceholderText(QStringLiteral("输入内容后，回车修改成功"));
			}
			});
		});
	(void)connect(ui.btnNewGrouptodo, &QPushButton::clicked, this, [=]() {
		ui.stackedWidgetGroupNewTodo->setCurrentIndex(1);
		ui.lineEditNewGrouptodo->setPlaceholderText(QStringLiteral("输入内容后，回车创建成功")); // 设置输入框的提示文本
		connect(ui.lineEditNewGrouptodo, &QLineEdit::textChanged, this, [=](const QString& text) {
			if (!text.isEmpty()) { // 如果输入框不为空，隐藏提示文本
				ui.lineEditNewGrouptodo->setPlaceholderText("");
			}
			else { // 如果输入框为空，显示提示文本 
				ui.lineEditNewGrouptodo->setPlaceholderText(QStringLiteral("输入内容后，回车创建成功"));
			}
			});
		});
	(void)connect(ui.btnNewGroup, &QPushButton::clicked, this, [=]() {
		ui.stackedWidgetGroupNew->setCurrentIndex(1);
		ui.lineEditNewGroup->setPlaceholderText(QStringLiteral("输入内容后，回车创建成功")); // 设置输入框的提示文本

		connect(ui.lineEditNewGroup, &QLineEdit::textChanged, this, [=](const QString& text) {
			if (!text.isEmpty()) { // 如果输入框不为空，隐藏提示文本
				ui.lineEditNewGroup->setPlaceholderText("");
			}
			else { // 如果输入框为空，显示提示文本 
				ui.lineEditNewGroup->setPlaceholderText(QStringLiteral("输入内容后，回车创建成功"));
			}
			});
		});
	(void)connect(ui.btnNewGroupMem, &QPushButton::clicked, this, [=]() {
		ui.stackedWidgetSend->setCurrentIndex(1);
		ui.lineEditSend->setPlaceholderText(QStringLiteral("输入邀请人姓名后，回车发送邀请")); // 设置输入框的提示文本

		connect(ui.lineEditSend, &QLineEdit::textChanged, this, [=](const QString& text) {
			if (!text.isEmpty()) { // 如果输入框不为空，隐藏提示文本
				ui.lineEditSend->setPlaceholderText("");
			}
			else { // 如果输入框为空，显示提示文本 
				ui.lineEditSend->setPlaceholderText(QStringLiteral("输入邀请人姓名后，回车发送邀请"));
			}
			});
		});
	(void)connect(ui.lineEditGroupName, &QLineEdit::returnPressed, this, [=]() {
		int group_id = ui.treeWidgetLeft->currentItem()->data(0, Qt::UserRole).toUInt();
		// 在这里执行任务打开操作
		if (group_id != 0)
		{
			QString groupName = ui.lineEditGroupName->text();
			if (m_service->updateGroupName(group_id, groupName))
			{
				m_dialog->setMsg(QStringLiteral("修改成功"), QStringLiteral("您已经成功修改分组名字"));
				m_dialog->show();
				
				ui.btnRotate->click();
			}
			else
			{
				m_dialog->setMsg(QStringLiteral("修改失败"), QStringLiteral("请检查数据库连接"));
				m_dialog->show();

			}

		}
		ui.lineEditGroupName->clear();
		ui.stackedWidgetGroupNew->setCurrentIndex(0);
		
		});
	(void)connect(ui.lineEditNewGroup, &QLineEdit::returnPressed, this, [=]() {
		QString groupName = ui.lineEditNewGroup->text();
		if (m_service->createNewGroup(groupName, m_userId, m_username))
		{
			m_dialog->setMsg(QStringLiteral("创建成功"), QStringLiteral("您已经成功创建分组"));
			m_dialog->show();
			ui.stackedWidgetGroupNew->setCurrentIndex(0);
			ui.btnRotate->click();
		}
		else
		{
			m_dialog->setMsg(QStringLiteral("修改失败"), QStringLiteral("请检查数据库连接"));
			m_dialog->show();
			ui.stackedWidgetGroupNew->setCurrentIndex(0);
		}
		ui.lineEditNewGroup->clear();
		});
	(void)connect(ui.lineEditSend, &QLineEdit::returnPressed, this, [=]() {
		QString send_userName = ui.lineEditSend->text();
		int group_id = ui.treeWidgetLeft->currentItem()->data(0, Qt::UserRole).toUInt();
		// 在这里执行任务打开操作
		if (group_id != 0)
		{
			if (m_service->createInfo(send_userName, group_id))
			{
				m_dialog->setMsg(QStringLiteral("邀请发送成功"), QStringLiteral("您已经成功发送邀请"));
			}
			else
			{
				m_dialog->setMsg(QStringLiteral("邀请发送失败"), QStringLiteral("请检查数据库连接"));
			}
		}
		m_dialog->show();
		ui.stackedWidgetSend->setCurrentIndex(0);
		ui.lineEditSend->clear();
		});
	(void)connect(ui.lineEditNewGrouptodo, &QLineEdit::returnPressed, this, [=]() {
		QString task_name = ui.lineEditNewGrouptodo->text();
		int group_id = ui.treeWidgetLeft->currentItem()->data(0, Qt::UserRole).toUInt();
		// 在这里执行任务打开操作
		if (group_id == 0)
		{
			ui.stackedWidgetGroupNewTodo->setCurrentIndex(0);
		}
		if (m_service->createNewTaskByGroupId(group_id, task_name))
		{
			m_dialog->setMsg(QStringLiteral("创建成功"), QStringLiteral("您已经成功创建待办"));
			m_dialog->show();
			ui.stackedWidgetGroupNewTodo->setCurrentIndex(0);
			ui.btnRotate->click();
		}
		else
		{
			m_dialog->setMsg(QStringLiteral("修改失败"), QStringLiteral("请检查数据库连接"));
			m_dialog->show();
			ui.stackedWidgetGroupNewTodo->setCurrentIndex(0);
		}
		ui.lineEditNewGrouptodo->clear();
		});
	(void)connect(ui.btnDelete, &QPushButton::clicked, this, &KMainWindow::deleteGroup);
	(void)connect(ui.comboBoxGroupSort, QOverload<int>::of(&QComboBox::activated), this, [=]() {
		if (ui.comboBoxGroupSort->currentIndex() == 0)		// 默认排序
		{

		}
		else if (ui.comboBoxGroupSort->currentIndex() == 1)		// 按到期时间排序
		{

			// 将任务按到期时间从早到晚排序
			std::sort(m_AllTasks->begin(), m_AllTasks->end(), [](const KTask& a, const KTask& b) {
				return a.getDueDate() < b.getDueDate();
				});

		}
		else if (ui.comboBoxGroupSort->currentIndex() == 2)		// 按创建时间排序
		{
			// 将任务按创建时间从早到晚排序
			std::sort(m_AllTasks->begin(), m_AllTasks->end(), [](const KTask& a, const KTask& b) {
				return a.getId() < b.getId();
				});
		}
		else               // 按优先级排序
		{
			// 将任务按优先级排序
			std::sort(m_AllTasks->begin(), m_AllTasks->end(), [](const KTask& a, const KTask& b) {
				return a.getPriority() > b.getPriority();
				});
		}
		ui.treeWidgetGroup->clear();

		});
}


void KMainWindow::btnNewToDoClicked()
{
	ui.stackedWidgetHomeNewtodo->setCurrentIndex(1);
	ui.lineEditNewTodo->setPlaceholderText(QStringLiteral("输入内容后，回车创建成功")); // 设置输入框的提示文本

	connect(ui.lineEditNewTodo, &QLineEdit::textChanged, this, [=](const QString& text) {
		if (!text.isEmpty()) { // 如果输入框不为空，隐藏提示文本
			ui.lineEditNewTodo->setPlaceholderText("");
		}
		else { // 如果输入框为空，显示提示文本
			ui.lineEditNewTodo->setPlaceholderText(QStringLiteral("输入内容后，回车创建成功"));
		}
		});

}
void KMainWindow::btnHomeDateClicked()
{
	ui.stackedWidgetDueDate->setCurrentIndex(1);
}
void KMainWindow::on_treeWidget_itemClicked(QTreeWidgetItem* item, int column)
{
	setUnDisableRight();
	ui.treeWidgetRight->clear();
	QVariant idVar = item->data(0, Qt::UserRole);
	int taskId = idVar.toInt();
	// 在这里执行任务打开操作
	if (taskId == 0)return;
	// 根据ID去右边设置详细值
	// 创建根节点
	QTreeWidgetItem* Root = new QTreeWidgetItem(ui.treeWidgetRight);
	KTask task;
	for (const auto taskGet : *m_AllTasks)
	{
		if (taskGet.getId() == taskId) {
			task = taskGet; 
		}
	}

	Root->setText(0, task.getEventName());
	ui.lineEditName->setText(task.getEventName());
	Root->setData(0, Qt::UserRole, QVariant(task.getId()));
	QString setText;
	QTreeWidgetItem* taskItemPriority = new QTreeWidgetItem();
	setText = QStringLiteral("优先级:") + checkPriority(task.getPriority());
	ui.comboBoxHome->setCurrentIndex(task.getPriority());
	taskItemPriority->setText(0, setText);
	Root->addChild(taskItemPriority);

	QTreeWidgetItem* taskItemGroup = new QTreeWidgetItem();
	setText = QStringLiteral("分组:") + checkGroup(task.getGroupId());
	taskItemGroup->setText(0, setText);
	Root->addChild(taskItemGroup);

	QTreeWidgetItem* taskItemDueDate = new QTreeWidgetItem();
	setText = QStringLiteral("截止日期:") + task.getDueDate().toString("yyyy-MM-dd hh:mm:ss");
	taskItemDueDate->setText(0, setText);
	Root->addChild(taskItemDueDate);

	QTreeWidgetItem* taskItemRemindTime = new QTreeWidgetItem();
	setText = QStringLiteral("提醒时间:") + task.getRemindTime().toString("yyyy-MM-dd hh:mm:ss");
	taskItemRemindTime->setText(0, setText);
	Root->addChild(taskItemRemindTime);


	QTreeWidgetItem* taskItemRepeted = new QTreeWidgetItem();
	if (task.isRepeated())
	{
		setText = QStringLiteral("是否重复:是");
	}
	else setText = QStringLiteral("是否重复:否");
	
	taskItemRepeted->setText(0, setText);
	Root->addChild(taskItemRepeted);


	QTreeWidgetItem* taskItemDescription = new QTreeWidgetItem();
	setText = QStringLiteral("描述:") + task.getDescription();
	taskItemDescription->setText(0, setText);
	Root->addChild(taskItemDescription);

	QTreeWidgetItem* taskItemLocation = new QTreeWidgetItem();
	setText = QStringLiteral("地点:") + task.getLocation();
	taskItemLocation->setText(0, setText);
	Root->addChild(taskItemLocation);

	QTreeWidgetItem* taskItemNote = new QTreeWidgetItem();
	setText = QStringLiteral("备注:") + task.getNote();
	taskItemNote->setText(0, setText);
	Root->addChild(taskItemNote);
	ui.treeWidgetRight->expandAll();
	
}
void KMainWindow::on_treeWidgetLeft_itemClicked(QTreeWidgetItem* item, int column)
{
	ui.treeWidgetGroup->clear();
	QVariant idVar = item->data(0, Qt::UserRole);
	int group_id = idVar.toInt();
	// 在这里执行任务打开操作
	if (group_id == 0)return;
	// 根据ID去右边设置详细值
	// 创建根节点
	QTreeWidgetItem* haveFinishedRoot = new QTreeWidgetItem(ui.treeWidgetGroup);
	haveFinishedRoot->setText(0, QStringLiteral("已完成"));


	QTreeWidgetItem* Root = new QTreeWidgetItem(ui.treeWidgetGroup);
	Root->setText(0, QStringLiteral("我的待办"));
	Root->setExpanded(true);

	if (ui.comboBoxGroupSort->currentIndex() == 0)		// 默认排序
	{

	}
	else if (ui.comboBoxGroupSort->currentIndex() == 1)		// 按到期时间排序
	{

		// 将任务按到期时间从早到晚排序
		std::sort(m_AllTasks->begin(), m_AllTasks->end(), [](const KTask& a, const KTask& b) {
			return a.getDueDate() < b.getDueDate();
			});

	}
	else if (ui.comboBoxGroupSort->currentIndex() == 2)		// 按创建时间排序
	{
		// 将任务按创建时间从早到晚排序
		std::sort(m_AllTasks->begin(), m_AllTasks->end(), [](const KTask& a, const KTask& b) {
			return a.getId() < b.getId();
			});
	}
	else               // 按优先级排序
	{
		// 将任务按优先级排序
		std::sort(m_AllTasks->begin(), m_AllTasks->end(), [](const KTask& a, const KTask& b) {
			return a.getPriority() > b.getPriority();
			});
	}
	// 添加子项
	for (const auto task : *m_AllTasks) {
		if (task.getGroupId() == group_id)
		{
			QTreeWidgetItem* taskItem = new QTreeWidgetItem();
			QString taskMes;
			taskMes = task.getEventName();
			int groupId = task.getGroupId();
			QString groupStr = checkGroup(groupId);
			taskMes += QStringLiteral(" 分组:") + groupStr;
			int priority = task.getPriority();
			QString priorityStr = checkPriority(priority);

			taskMes += QStringLiteral(" 优先级:") + priorityStr;
			taskItem->setText(0, taskMes);
			taskItem->setData(0, Qt::UserRole, QVariant(task.getId()));
			if (task.getDueDate().isValid()
				&& task.getDueDate() < QDateTime::currentDateTime())
			{
				haveFinishedRoot->addChild(taskItem);
			}
			else Root->addChild(taskItem);
		}
	}

}

void KMainWindow::on_treeWidgetGroup_itemClicked(QTreeWidgetItem* item, int column)
{
	setUnDisableRight();
	ui.treeWidgetRight->clear();
	QVariant idVar = item->data(0, Qt::UserRole);
	int taskId = idVar.toInt();
	// 在这里执行任务打开操作
	if (taskId == 0)return;
	// 根据ID去右边设置详细值
	// 创建根节点
	QTreeWidgetItem* Root = new QTreeWidgetItem(ui.treeWidgetRight);
	KTask task;
	for (const auto taskGet : *m_AllTasks)
	{
		if (taskGet.getId() == taskId) {
			task = taskGet;
		}
	}

	Root->setText(0, task.getEventName());
	ui.lineEditName->setText(task.getEventName());
	Root->setData(0, Qt::UserRole, QVariant(task.getId()));
	QString setText;
	QTreeWidgetItem* taskItemPriority = new QTreeWidgetItem();
	setText = QStringLiteral("优先级:") + checkPriority(task.getPriority());
	ui.comboBoxHome->setCurrentIndex(task.getPriority());
	taskItemPriority->setText(0, setText);
	Root->addChild(taskItemPriority);

	QTreeWidgetItem* taskItemGroup = new QTreeWidgetItem();
	setText = QStringLiteral("分组:") + checkGroup(task.getGroupId());
	taskItemGroup->setText(0, setText);
	Root->addChild(taskItemGroup);

	QTreeWidgetItem* taskItemDueDate = new QTreeWidgetItem();
	setText = QStringLiteral("截止日期:") + task.getDueDate().toString("yyyy-MM-dd hh:mm:ss");
	taskItemDueDate->setText(0, setText);
	Root->addChild(taskItemDueDate);

	QTreeWidgetItem* taskItemRemindTime = new QTreeWidgetItem();
	setText = QStringLiteral("提醒时间:") + task.getRemindTime().toString("yyyy-MM-dd hh:mm:ss");
	taskItemRemindTime->setText(0, setText);
	Root->addChild(taskItemRemindTime);


	QTreeWidgetItem* taskItemRepeted = new QTreeWidgetItem();
	if (task.isRepeated())
	{
		setText = QStringLiteral("是否重复:是");
	}
	else setText = QStringLiteral("是否重复:否");

	taskItemRepeted->setText(0, setText);
	Root->addChild(taskItemRepeted);


	QTreeWidgetItem* taskItemDescription = new QTreeWidgetItem();
	setText = QStringLiteral("描述:") + task.getDescription();
	taskItemDescription->setText(0, setText);
	Root->addChild(taskItemDescription);

	QTreeWidgetItem* taskItemLocation = new QTreeWidgetItem();
	setText = QStringLiteral("地点:") + task.getLocation();
	taskItemLocation->setText(0, setText);
	Root->addChild(taskItemLocation);

	QTreeWidgetItem* taskItemNote = new QTreeWidgetItem();
	setText = QStringLiteral("备注:") + task.getNote();
	taskItemNote->setText(0, setText);
	Root->addChild(taskItemNote);
	ui.treeWidgetRight->expandAll();

}

void KMainWindow::changeHomeDueDate()
{
	int task_id;
	if (ui.btnGroup->isChecked()) task_id = ui.treeWidgetGroup->currentItem()->data(0, Qt::UserRole).toUInt();
	else task_id = ui.treeWidget->currentItem()->data(0, Qt::UserRole).toUInt();
	if (task_id == 0)
	{
		ui.stackedWidgetDueDate->setCurrentIndex(0);
		return;
	}
	QDateTime selectedDateTime = ui.dateTimeEditHome->dateTime();
	QDateTime currentDateTime = QDateTime::currentDateTime();

	qint64 selectedTimestamp = selectedDateTime.toSecsSinceEpoch();
	qint64 currentTimestamp = currentDateTime.toSecsSinceEpoch();
	
		if (m_service->updateDueDate(task_id, selectedDateTime))
		{
			m_dialog->setMsg(QStringLiteral("修改成功"), QStringLiteral("您已经修改截止时间"));
			m_dialog->show();
			ui.stackedWidgetDueDate->setCurrentIndex(0);
			ui.btnRotate->click();
		}
		else
		{
			m_dialog->setMsg(QStringLiteral("修改失败"), QStringLiteral("请检查数据库连接"));
			m_dialog->show();
			ui.stackedWidgetDueDate->setCurrentIndex(0);
		}
	
}

void KMainWindow::changeHomeRemind_time()
{
	int task_id;
	if (ui.btnGroup->isChecked()) task_id = ui.treeWidgetGroup->currentItem()->data(0, Qt::UserRole).toUInt();
	else task_id = ui.treeWidget->currentItem()->data(0, Qt::UserRole).toUInt();
	if (task_id == 0)
	{
		ui.stackedWidgetRemind->setCurrentIndex(0);
		return;
	}
	KTask task;
	for (const auto taskGet : *m_AllTasks)
	{
		if (taskGet.getId() == task_id) {
			task = taskGet;
		}
	}
	
	QDateTime dueDateTime = task.getDueDate();
	QDateTime selectedDateTime = ui.dateTimeEditHome_2->dateTime();
	QDateTime currentDateTime = QDateTime::currentDateTime();

	qint64 selectedTimestamp = selectedDateTime.toSecsSinceEpoch();
	qint64 currentTimestamp = currentDateTime.toSecsSinceEpoch();
	qint64 dueDateTimestamp = currentTimestamp;
	if (dueDateTime.isValid())
	{
		dueDateTimestamp = dueDateTime.toSecsSinceEpoch();
	}
	if (selectedTimestamp < currentTimestamp)
	{
		m_dialog->setMsg(QStringLiteral("修改失败"), QStringLiteral("选择时间不能早于当前时间"));
		m_dialog->show();
		ui.stackedWidgetRemind->setCurrentIndex(0);
		//ui.btnRotate->click();
	}
	else if (dueDateTime.isValid() && selectedTimestamp > dueDateTimestamp)
	{
		m_dialog->setMsg(QStringLiteral("修改失败"), QStringLiteral("选择时间不能晚于截止时间"));
		m_dialog->show();
		ui.stackedWidgetRemind->setCurrentIndex(0);
	}
	else
	{
		bool is_repeated = ui.checkBoxHome_Repeat->isChecked();
		if (m_service->updateRemindTime(task_id, selectedDateTime, is_repeated))
		{
			m_dialog->setMsg(QStringLiteral("修改成功"), QStringLiteral("您已经修改提醒时间"));
			m_dialog->show();
			ui.stackedWidgetRemind->setCurrentIndex(0);
			ui.btnRotate->click();
		}
		else
		{
			m_dialog->setMsg(QStringLiteral("修改失败"), QStringLiteral("请检查数据库连接"));
			m_dialog->show();
			ui.stackedWidgetRemind->setCurrentIndex(0);
		}
	}
}

void KMainWindow::on_treeWidgetRight_itemClicked(QTreeWidgetItem* item, int column)
{
	ui.textBrowserHomeBigger->clear();
	ui.textBrowserHomeBigger->setText(item->text(column));
	ui.stackedWidgetRightMain->setCurrentIndex(1);
	
}



void KMainWindow::btnRotateClicked()
{
	ui.stackedWidgetDueDate->setCurrentIndex(0);
	initTaskList();
	
}


void KMainWindow::lineEditNameChange()
{
	int task_id;
	if (ui.btnGroup->isChecked()) task_id = ui.treeWidgetGroup->currentItem()->data(0, Qt::UserRole).toUInt();
	else task_id = ui.treeWidget->currentItem()->data(0, Qt::UserRole).toUInt();
	if (task_id == 0) return;
	QString new_name = ui.lineEditName->text();
	if (m_service->taskNameChange(task_id, new_name))
	{
		m_dialog->setMsg(QStringLiteral("修改成功"), QStringLiteral("您已经修改待办事件名称"));
		m_dialog->show();
		ui.btnRotate->click();
	}
	else
	{
		m_dialog->setMsg(QStringLiteral("修改失败"), QStringLiteral("检查数据库连接"));
		m_dialog->show();
	}
}

void KMainWindow::btnHomeRemindClicked()
{
	int task_id;
	if (ui.btnGroup->isChecked()) task_id = ui.treeWidgetGroup->currentItem()->data(0, Qt::UserRole).toUInt();
	else task_id = ui.treeWidget->currentItem()->data(0, Qt::UserRole).toUInt();
	if (task_id == 0) return;
	ui.stackedWidgetRemind->setCurrentIndex(1);
}

void KMainWindow::btnHomeDescriptionClicked(bool checked)
{
	if (checked)
	{
		int task_id;
		if (ui.btnGroup->isChecked()) task_id = ui.treeWidgetGroup->currentItem()->data(0, Qt::UserRole).toUInt();
		else task_id = ui.treeWidget->currentItem()->data(0, Qt::UserRole).toUInt();
		if (task_id == 0)
		{
			ui.btnHomeDescription->setChecked(false);
			return;
		}
		ui.textEditHome->clear();
		ui.stackedWidgetRightMain->setCurrentIndex(2);
	}

}

void KMainWindow::btnHomeLocationClicked(bool checked)
{
	if (checked)
	{
		int task_id;
		if (ui.btnGroup->isChecked()) task_id = ui.treeWidgetGroup->currentItem()->data(0, Qt::UserRole).toUInt();
		else task_id = ui.treeWidget->currentItem()->data(0, Qt::UserRole).toUInt();
		if (task_id == 0)
		{
			ui.btnHomeLocation->setChecked(false);
			return;
		}
		ui.textEditHome->clear();
		ui.stackedWidgetRightMain->setCurrentIndex(2);
	}
}

void KMainWindow::comboBoxHomeChange(int index)
{
	int task_id;
	if (ui.btnGroup->isChecked()) task_id = ui.treeWidgetGroup->currentItem()->data(0, Qt::UserRole).toUInt();
	else task_id = ui.treeWidget->currentItem()->data(0, Qt::UserRole).toUInt();
	if (task_id == 0) return;
	if (m_service->updatePriority(task_id, index))
	{
		m_dialog->setMsg(QStringLiteral("设置成功"), QStringLiteral("您已经成功更改优先级"));
		m_dialog->show();
		ui.btnRotate->click();
	}
	else
	{
		m_dialog->setMsg(QStringLiteral("设置失败"), QStringLiteral("请检查数据库连接"));
		m_dialog->show();
	}
}

void KMainWindow::btnHomeNoteClicked(bool checked)
{
	if (checked)
	{
		int task_id;
		if (ui.btnGroup->isChecked()) task_id = ui.treeWidgetGroup->currentItem()->data(0, Qt::UserRole).toUInt();
		else task_id = ui.treeWidget->currentItem()->data(0, Qt::UserRole).toUInt();
		if (task_id == 0)
		{
			ui.btnHomeNote->setChecked(false);
			return;
		}
		ui.textEditHome->clear();
		ui.stackedWidgetRightMain->setCurrentIndex(2);
	}
}

void KMainWindow::btnHomeOKClicked()
{
	int task_id;
	if (ui.btnGroup->isChecked()) task_id = ui.treeWidgetGroup->currentItem()->data(0, Qt::UserRole).toUInt();
	else task_id = ui.treeWidget->currentItem()->data(0, Qt::UserRole).toUInt();
	if (task_id == 0) return;
	QString msg = ui.textEditHome->toPlainText();
	if (ui.btnHomeDescription->isChecked())
	{
		if (m_service->updateDescription(task_id, msg))
		{
			m_dialog->setMsg(QStringLiteral("设置成功"), QStringLiteral("您已经成功设置描述"));
			m_dialog->show();
		}
		else
		{
			m_dialog->setMsg(QStringLiteral("设置失败"), QStringLiteral("请检查数据库连接"));
			m_dialog->show();
		}
	}
	else if (ui.btnHomeLocation->isChecked())
	{
		if (m_service->updateLocation(task_id, msg))
		{
			m_dialog->setMsg(QStringLiteral("设置成功"), QStringLiteral("您已经成功设置地点"));
			m_dialog->show();
		}
		else
		{
			m_dialog->setMsg(QStringLiteral("设置失败"), QStringLiteral("请检查数据库连接"));
			m_dialog->show();
		}
	}
	else if (ui.btnHomeNote->isChecked())
	{
		if (m_service->updateNote(task_id, msg))
		{
			m_dialog->setMsg(QStringLiteral("设置成功"), QStringLiteral("您已经成功设置备注"));
			m_dialog->show();
		}
		else
		{
			m_dialog->setMsg(QStringLiteral("设置失败"), QStringLiteral("请检查数据库连接"));
			m_dialog->show();
		}
	}
	ui.stackedWidgetRightMain->setCurrentIndex(0);
	ui.btnRotate->click();

}

void KMainWindow::btnAcceptClicked()
{
	int group_id = m_infoMap->firstKey();
	if (m_service->acceptInfo(group_id, m_userId, m_username))
	{
		ui.stackedWidgetGroupAll->setCurrentIndex(0);
		auto it = m_infoMap->begin(); // 返回第一个元素的迭代器
		m_infoMap->erase(it);
	}
	else
	{
		ui.stackedWidgetGroupAll->setCurrentIndex(0);
		auto it = m_infoMap->begin(); // 返回第一个元素的迭代器
		m_infoMap->erase(it);
	}
	checkHaveInvite();
}

void KMainWindow::btnRefuseClicked()
{
	int group_id = m_infoMap->firstKey();
	if (m_service->refuseInfo(group_id, m_userId))
	{
		ui.stackedWidgetGroupAll->setCurrentIndex(0);
		auto it = m_infoMap->begin(); // 返回第一个元素的迭代器
		m_infoMap->erase(it);
	}
	else
	{
		ui.stackedWidgetGroupAll->setCurrentIndex(0);
		auto it = m_infoMap->begin(); // 返回第一个元素的迭代器
		m_infoMap->erase(it);
	}
	checkHaveInvite();
}

void KMainWindow::deleteGroup()
{
	int group_id = ui.treeWidgetLeft->currentItem()->data(0, Qt::UserRole).toUInt();
	// 在这里执行任务打开操作
	if (group_id == 0)return;
	if (m_service->deleteUsersGroup(group_id, m_userId, m_username))
	{
		m_dialog->setMsg(QStringLiteral("删除成功"), QStringLiteral("您已经成功删除该分组"));
		m_dialog->show();
		ui.btnRotateGroup->click();
	}
	else
	{
		m_dialog->setMsg(QStringLiteral("删除失败"), QStringLiteral("请检查数据库连接"));
		m_dialog->show();
	}

}

void KMainWindow::onAllTasksQueryFinished(QList<KTask> tasks)
{
	// 分类
	m_AllTasks->clear();
	m_AllTasks->append(tasks);
	m_unscheduledTasks->clear();
	m_HaveFinishedTasks->clear();
	m_next7DaysTasks->clear();
	m_afterNextWeekTasks->clear();
	delete m_service;
	m_service = new KService(this);
	for (const auto& task : *m_AllTasks)
	{
		if (!task.isArranged())
			m_unscheduledTasks->append(task);
		else if (task.getDueDate() <= QDateTime::currentDateTime())
			m_HaveFinishedTasks->append(task);
		else if (task.getDueDate() > QDateTime::currentDateTime()
			&& task.getDueDate() < QDateTime::currentDateTime().addDays(7))
			m_next7DaysTasks->append(task);
		else
			m_afterNextWeekTasks->append(task);
	}
	//initLeftWidget();
	// 更新UI
	ui.treeWidgetRight->clear();
	ui.lineEditName->clear();
	ui.comboBoxHome->setCurrentIndex(0);
	initLeftWidget();
	ui.stackedWidgetMid->setDisabled(false);
	if (ui.btnHome->isChecked())
		initHomeListWidget();
	else if (ui.btnTodo->isChecked())
		initMyListWidget();
	else if (ui.btnGroup->isChecked())
		initGroupWidget();
}
