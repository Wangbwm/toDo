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
	void btnLoginClicked();		// ��¼��ť�ۺ���
	void btnRegisterClicked();		// ע�ᰴť�ۺ���
	void btnLoginEyeChange(bool checked);
	void btnRegisterClickedEyeChange(bool checked);
	void btnRegisterClickedEyeChange_2(bool checked);
private:
	Ui::KLoginWindowClass ui;
	KService* m_service;		// ҵ������
	KDialogWindow* m_dialog;		// ��������
	QString m_username;
	void init();		// ��ʼ��
	void initService();		// ��ʼ��ҵ������
	void initConnect();		// ��ʼ���źźͲۺ���
};
#endif // KLOGINWINDOW_H