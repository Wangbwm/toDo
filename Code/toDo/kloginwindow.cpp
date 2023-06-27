#include "kloginwindow.h"
#include <QCryptographicHash>

KLoginWindow::KLoginWindow(QWidget *parent)
	: QWidget(parent)
	, m_service(nullptr)
	, m_dialog(nullptr)
{
	ui.setupUi(this);
	init();
	initService();
	initConnect();
}

KLoginWindow::~KLoginWindow()
{
	delete m_service;
	delete m_dialog;
}


// ��ʼ��
void KLoginWindow::init()
{
	m_dialog = new KDialogWindow(nullptr);
	setWindowTitle(QStringLiteral("��¼/ע��"));
	ui.stackedWidget->setCurrentIndex(0);
	ui.lineEditPwd->setEchoMode(QLineEdit::Password);
	ui.lineEditRePwd->setEchoMode(QLineEdit::Password);
	ui.lineEditRePwd_2->setEchoMode(QLineEdit::Password);

}
// ��ʼ��ҵ��ӿ�
void KLoginWindow::initService()
{
	m_service = new KService(nullptr);
}
// ��ʼ���źźͲۺ���
void KLoginWindow::initConnect()
{
	(void)connect(ui.btnLogin, &QPushButton::clicked, this, &KLoginWindow::btnLoginClicked);		// ��¼�źŲ�
	(void)connect(ui.btnRegister, &QPushButton::clicked, this, &KLoginWindow::btnRegisterClicked);		// ע���źŲ�
	(void)connect(ui.btnToRegister, &QPushButton::clicked, this, [=]() {		// ��ҳ��ת�źŲ�
		this->ui.stackedWidget->setCurrentIndex(1);
		});
	(void)connect(ui.btnToLogin, &QPushButton::clicked, this, [=]() {		// ��ҳ��ת�źŲ�
		this->ui.stackedWidget->setCurrentIndex(0);
		});
	(void)connect(m_dialog, &KDialogWindow::sendToLoginWindow, this, [=]() {		// ���ܵ����ź�
		this->close();
		this->sendToMainWindow(m_dialog, m_username);
		});
	(void)connect(ui.btnShowPwd, &QPushButton::toggled, this, &KLoginWindow::btnLoginEyeChange);
	(void)connect(ui.btnShowPwd_2, &QPushButton::toggled, this, &KLoginWindow::btnRegisterClickedEyeChange);
	(void)connect(ui.btnShowPwd_3, &QPushButton::toggled, this, &KLoginWindow::btnRegisterClickedEyeChange_2);
	(void)connect(ui.lineEditPwd, &QLineEdit::returnPressed, this, [=]() {
		btnLoginClicked();
		});
	(void)connect(ui.lineEditRePwd_2, &QLineEdit::returnPressed, this, [=]() {
		btnRegisterClicked();
		});
}
/**
  * �������MD5���ܣ�Ϊ�˷�ֹ���ݿ�洢���������뱻��������ȡ��������й¶��
  ͨ����Ҫ�����ݿ��д洢�����뾭����ϣ������ٴ洢��
  ����ϣ�㷨���򲻿�������ԣ�ʹ�ü�ʹ�����߻�ȡ����ϣֵҲ���Ը��ݹ�ϣֵ���Ƴ�ԭʼ���롣
  */
void KLoginWindow::btnLoginClicked()
{
	QString username = ui.lineEditUserName->text();
	QString password = ui.lineEditPwd->text();
	QByteArray hash = QCryptographicHash::hash(QVariant(password).toByteArray(), QCryptographicHash::Md5);
	password = hash.toHex();
	if (m_service->login(username, password))
	{
		m_dialog->setMsg(QStringLiteral("��¼�ɹ�"), QStringLiteral("��ϲ���½�ɹ�!"));
		m_dialog->show();
		m_username = username;

	}
	else
	{
		m_dialog->setMsg(QStringLiteral("�������"), QStringLiteral("��������ȷ����!"));
		m_dialog->show();
	}
}
void KLoginWindow::btnRegisterClicked()
{
	QString username = ui.lineEditReName->text();
	QString password = ui.lineEditRePwd->text();
	QString rePwd = ui.lineEditRePwd_2->text();
	if (username == "" || password == "" || rePwd == "")
	{
		m_dialog->setMsg(QStringLiteral("�������"), QStringLiteral("�˺ź����벻��Ϊ��!"));
		m_dialog->show();
		return;
	}
	if (password != rePwd)
	{
		m_dialog->setMsg(QStringLiteral("�������"), QStringLiteral("�������벻���!"));
		m_dialog->show();
		return;
	}
	QByteArray hash = QCryptographicHash::hash(QVariant(password).toByteArray(), QCryptographicHash::Md5);
	password = hash.toHex();
	if (m_service->regist(username, password))
	{
		m_dialog->setMsg(QStringLiteral("ע��ɹ�"), QStringLiteral("��ϲ��ע��ɹ�!"));
		m_dialog->show();
	}
	else
	{
		m_dialog->setMsg(QStringLiteral("ע��ʧ��"), QStringLiteral("�������û�����ע��!"));
		m_dialog->show();

	}
}
// ��ʾ�������������
void KLoginWindow::btnLoginEyeChange(bool checked)
{
	if (checked)
	{
		ui.btnShowPwd->setStyleSheet("border-image:url(:/images/eye.svg);");
		ui.lineEditPwd->setEchoMode(QLineEdit::Normal);
	}
		
	else
	{
		ui.btnShowPwd->setStyleSheet("border-image: url(:/images/eye-slash.svg);");
		ui.lineEditPwd->setEchoMode(QLineEdit::Password);
	}
}
// ��ʾ�������������
void KLoginWindow::btnRegisterClickedEyeChange(bool checked)
{
	if (checked)
	{
		ui.btnShowPwd_2->setStyleSheet("border-image:url(:/images/eye.svg);");
		ui.lineEditRePwd->setEchoMode(QLineEdit::Normal);

	}

	else
	{
		ui.btnShowPwd_2->setStyleSheet("border-image: url(:/images/eye-slash.svg);");
		ui.lineEditRePwd->setEchoMode(QLineEdit::Password);

	}
}
// ��ʾ�������������
void KLoginWindow::btnRegisterClickedEyeChange_2(bool checked)
{
	if (checked)
	{
		ui.btnShowPwd_3->setStyleSheet("border-image:url(:/images/eye.svg);");
		ui.lineEditRePwd_2->setEchoMode(QLineEdit::Normal);
	}

	else
	{
		ui.btnShowPwd_3->setStyleSheet("border-image: url(:/images/eye-slash.svg);");
		ui.lineEditRePwd_2->setEchoMode(QLineEdit::Password);
	}
}
