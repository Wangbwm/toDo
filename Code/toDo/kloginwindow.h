#ifndef KLOGINWINDOW_H
#define KLOGINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include "ui_kloginwindow.h"
#include "kservice.h"
#include "kdialogwindow.h"

class KLoginWindow : public QWidget
{
	Q_OBJECT

public:
	KLoginWindow(QWidget *parent = nullptr);
	~KLoginWindow();
signals:
	void sendToMainWindow(KDialogWindow* dialog, QString username);
private slots:
	void btnLoginClicked();		// 登录按钮槽函数
	void btnRegisterClicked();		// 注册按钮槽函数
	void btnLoginEyeChange(bool checked);
	void btnRegisterClickedEyeChange(bool checked);
	void btnRegisterClickedEyeChange_2(bool checked);
private:
	Ui::KLoginWindowClass ui;
	KService* m_service;		// 业务层对象
	KDialogWindow* m_dialog;		// 弹窗对象
	QString m_username;
	void init();		// 初始化
	void initService();		// 初始化业务层对象
	void initConnect();		// 初始化信号和槽函数
};
#endif // KLOGINWINDOW_H