#include "kdialogwindow.h"

KDialogWindow::KDialogWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	initConnect();
}

KDialogWindow::~KDialogWindow()
{}
// 初始化信号和槽函数
void KDialogWindow::initConnect()
{
	(void)connect(ui.btnOK, &QPushButton::clicked, this, [=]() {
		this->close();
		if (this->ui.labelMSG->text() == QStringLiteral("恭喜你登陆成功!"))sendToLoginWindow();
		});
}
// 设置弹窗信息
void KDialogWindow::setMsg(QString title, QString msg)
{
	setWindowTitle(title);
	ui.labelMSG->setAlignment(Qt::AlignCenter);	// 设置对齐方式
	ui.labelMSG->setText(msg);
}