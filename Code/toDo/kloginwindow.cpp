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


// 初始化
void KLoginWindow::init()
{
	m_dialog = new KDialogWindow(nullptr);
	setWindowTitle(QStringLiteral("登录/注册"));
	ui.stackedWidget->setCurrentIndex(0);
	ui.lineEditPwd->setEchoMode(QLineEdit::Password);
	ui.lineEditRePwd->setEchoMode(QLineEdit::Password);
	ui.lineEditRePwd_2->setEchoMode(QLineEdit::Password);

}
// 初始化业务接口
void KLoginWindow::initService()
{
	m_service = new KService(nullptr);
}
// 初始化信号和槽函数
void KLoginWindow::initConnect()
{
	(void)connect(ui.btnLogin, &QPushButton::clicked, this, &KLoginWindow::btnLoginClicked);		// 登录信号槽
	(void)connect(ui.btnRegister, &QPushButton::clicked, this, &KLoginWindow::btnRegisterClicked);		// 注册信号槽
	(void)connect(ui.btnToRegister, &QPushButton::clicked, this, [=]() {		// 分页跳转信号槽
		this->ui.stackedWidget->setCurrentIndex(1);
		});
	(void)connect(ui.btnToLogin, &QPushButton::clicked, this, [=]() {		// 分页跳转信号槽
		this->ui.stackedWidget->setCurrentIndex(0);
		});
	(void)connect(m_dialog, &KDialogWindow::sendToLoginWindow, this, [=]() {		// 接受弹窗信号
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
  * 密码采用MD5加密，为了防止数据库存储的明文密码被攻击者窃取导致密码泄露，
  通常需要把数据库中存储的密码经过哈希运算后再存储。
  而哈希算法单向不可逆的特性，使得即使攻击者获取到哈希值也难以根据哈希值反推出原始密码。
  */
void KLoginWindow::btnLoginClicked()
{
	QString username = ui.lineEditUserName->text();
	QString password = ui.lineEditPwd->text();
	QByteArray hash = QCryptographicHash::hash(QVariant(password).toByteArray(), QCryptographicHash::Md5);
	password = hash.toHex();
	if (m_service->login(username, password))
	{
		m_dialog->setMsg(QStringLiteral("登录成功"), QStringLiteral("恭喜你登陆成功!"));
		m_dialog->show();
		m_username = username;

	}
	else
	{
		m_dialog->setMsg(QStringLiteral("密码错误"), QStringLiteral("请输入正确密码!"));
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
		m_dialog->setMsg(QStringLiteral("输入错误"), QStringLiteral("账号和密码不能为空!"));
		m_dialog->show();
		return;
	}
	if (password != rePwd)
	{
		m_dialog->setMsg(QStringLiteral("输入错误"), QStringLiteral("两次密码不相等!"));
		m_dialog->show();
		return;
	}
	QByteArray hash = QCryptographicHash::hash(QVariant(password).toByteArray(), QCryptographicHash::Md5);
	password = hash.toHex();
	if (m_service->regist(username, password))
	{
		m_dialog->setMsg(QStringLiteral("注册成功"), QStringLiteral("恭喜你注册成功!"));
		m_dialog->show();
	}
	else
	{
		m_dialog->setMsg(QStringLiteral("注册失败"), QStringLiteral("可能是用户名已注册!"));
		m_dialog->show();

	}
}
// 显示密码和隐藏密码
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
// 显示密码和隐藏密码
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
// 显示密码和隐藏密码
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
