#ifndef KDIALOGWINDOW_H
#define KDIALOGWINDOW_H

#include <QWidget>
#include "ui_kdialogwindow.h"
#include <QPushButton>
class KDialogWindow : public QWidget
{
	Q_OBJECT

public:
	KDialogWindow(QWidget *parent = nullptr);
	~KDialogWindow();
	void setMsg(QString title, QString msg);  // 设置弹窗信息
signals:
	void sendToLoginWindow();		// 发往登录页面的信号
private:
	Ui::KDialogWindowClass ui;
	void initConnect();
};
#endif // KDIALOGWINDOW_H