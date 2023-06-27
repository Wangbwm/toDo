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
	void btnNewToDoClicked();		// �½����찴ť����ۺ���
	void btnHomeDateClicked();		// ���½�ֹ���ڰ�ť����ۺ���
	void on_treeWidget_itemClicked(QTreeWidgetItem* item, int column);		// ��ͼ�������

	void changeHomeDueDate();		// ���½�ֹ����
	void changeHomeRemind_time();		// ������������
	void on_treeWidgetRight_itemClicked(QTreeWidgetItem* item, int column);		// �Ҳ���ϸ��ͼ�������
	void on_treeWidgetLeft_itemClicked(QTreeWidgetItem* item, int column);		// ��������ͼ�������
	void on_treeWidgetGroup_itemClicked(QTreeWidgetItem* item, int column);		// ����ҳ����ͼ�������
	void btnRotateClicked();		// ˢ�°�ť���
	void lineEditNameChange();		// 
	void btnHomeRemindClicked();		// �����������ڰ�ť���
	void btnHomeDescriptionClicked(bool checked);		// ����������ť���
	void btnHomeLocationClicked(bool checked);		// ���µص㰴ť���
	void comboBoxHomeChange(int index);		// ���ȼ�ѡ�����
	void btnHomeNoteClicked(bool checked);		// ���±�ע��ť���
	void btnHomeOKClicked();		// 
	void btnAcceptClicked();		// ����������
	void btnRefuseClicked();		// �ܾ�������
	void deleteGroup();		// ɾ����
	void onAllTasksQueryFinished(QList<KTask> tasks);
private:
    Ui::KMainWindowClass ui;
	KLoginWindow* m_loginWindow;
	KDialogWindow* m_dialog;      // ��������
	KService* m_service;        // ҵ������
	QString m_username;
	int m_userId;
	QList<KTask>* m_unscheduledTasks; // δ����
	QList<KTask>* m_next7DaysTasks; // δ��7��
	QList<KTask>* m_afterNextWeekTasks; // �Ժ�
	QList<KTask>* m_HaveFinishedTasks;
	QList<KTask>* m_AllTasks; // all
	QMap<int, QString>* m_usersGroup;
	QMap<int, QString>* m_infoMap;
	void init();        // ��ʼ��
	void initConnect();     // ��ʼ���źźͲۺ���
	void initConnectHome();		// ��ʼ����ҳ�źźͲۺ���
	void initConnectGroup();		// ��ʼ������ҳ���źźͲۺ���
	void initHomeListWidget();		// ��ʼ����ҳ��ͼ
	void initMyListWidget();		// ��ʼ�����ҵĴ��족��ͼ
	void initGroupWidget();		// ��ʼ������ҳ����ͼ
	void initLeftWidget();		// ��ʼ����������ͼ
	void initTaskList();		// ��ʼ����������
	void checkHaveInvite();		//	����Ƿ���������Ϣ
	void setDisableRight();		// �Ҳ����������Ϊ����ѡ
	void setUnDisableRight();		// �Ҳ����������Ϊ��ѡ
	QString checkGroup(int groupId);		// 
	QString checkPriority(int priority);		// 
};
#endif // KMAINWINDOW_H