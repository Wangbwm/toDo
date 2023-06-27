#ifndef KMAINWINDOW_H
#define KMAINWINDOW_H

#include <QtWidgets/QWidget>
#include "ui_kmainwindow.h"
#include "kloginwindow.h"
#include "ktask.h"

class KMainWindow : public QWidget
{
    Q_OBJECT

public:
    KMainWindow(QWidget *parent = nullptr);
    ~KMainWindow();

private slots:
	void btnNewToDoClicked();		// 新建待办按钮点击槽函数
	void btnHomeDateClicked();		// 更新截止日期按钮点击槽函数
	void on_treeWidget_itemClicked(QTreeWidgetItem* item, int column);		// 视图点击处理

	void changeHomeDueDate();		// 更新截止日期
	void changeHomeRemind_time();		// 更新提醒日期
	void on_treeWidgetRight_itemClicked(QTreeWidgetItem* item, int column);		// 右侧详细视图点击处理
	void on_treeWidgetLeft_itemClicked(QTreeWidgetItem* item, int column);		// 左侧分组视图点击处理
	void on_treeWidgetGroup_itemClicked(QTreeWidgetItem* item, int column);		// 分组页面视图点击处理
	void btnRotateClicked();		// 刷新按钮点击
	void lineEditNameChange();		// 
	void btnHomeRemindClicked();		// 更新提醒日期按钮点击
	void btnHomeDescriptionClicked(bool checked);		// 更新描述按钮点击
	void btnHomeLocationClicked(bool checked);		// 更新地点按钮点击
	void comboBoxHomeChange(int index);		// 优先级选择更改
	void btnHomeNoteClicked(bool checked);		// 更新备注按钮点击
	void btnHomeOKClicked();		// 
	void btnAcceptClicked();		// 接受邀请点击
	void btnRefuseClicked();		// 拒绝邀请点击
	void deleteGroup();		// 删除组
	void onAllTasksQueryFinished(QList<KTask> tasks);
private:
    Ui::KMainWindowClass ui;
	KLoginWindow* m_loginWindow;
	KDialogWindow* m_dialog;      // 弹窗对象
	KService* m_service;        // 业务层对象
	QString m_username;
	int m_userId;
	QList<KTask>* m_unscheduledTasks; // 未安排
	QList<KTask>* m_next7DaysTasks; // 未来7天
	QList<KTask>* m_afterNextWeekTasks; // 以后
	QList<KTask>* m_HaveFinishedTasks;
	QList<KTask>* m_AllTasks; // all
	QMap<int, QString>* m_usersGroup;
	QMap<int, QString>* m_infoMap;
	void init();        // 初始化
	void initConnect();     // 初始化信号和槽函数
	void initConnectHome();		// 初始化主页信号和槽函数
	void initConnectGroup();		// 初始化分组页面信号和槽函数
	void initHomeListWidget();		// 初始化主页视图
	void initMyListWidget();		// 初始化“我的待办”视图
	void initGroupWidget();		// 初始化分组页面视图
	void initLeftWidget();		// 初始化左侧分组视图
	void initTaskList();		// 初始化数据容器
	void checkHaveInvite();		//	检查是否有邀请信息
	void setDisableRight();		// 右侧更改栏设置为不可选
	void setUnDisableRight();		// 右侧更改栏设置为可选
	QString checkGroup(int groupId);		// 
	QString checkPriority(int priority);		// 
};
#endif // KMAINWINDOW_H