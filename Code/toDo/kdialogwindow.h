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
	void setMsg(QString title, QString msg);  // ���õ�����Ϣ
signals:
	void sendToLoginWindow();		// ������¼ҳ����ź�
private:
	Ui::KDialogWindowClass ui;
	void initConnect();
};
#endif // KDIALOGWINDOW_H