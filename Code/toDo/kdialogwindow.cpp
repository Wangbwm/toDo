#include "kdialogwindow.h"

KDialogWindow::KDialogWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	initConnect();
}

KDialogWindow::~KDialogWindow()
{}
// ��ʼ���źźͲۺ���
void KDialogWindow::initConnect()
{
	(void)connect(ui.btnOK, &QPushButton::clicked, this, [=]() {
		this->close();
		if (this->ui.labelMSG->text() == QStringLiteral("��ϲ���½�ɹ�!"))sendToLoginWindow();
		});
}
// ���õ�����Ϣ
void KDialogWindow::setMsg(QString title, QString msg)
{
	setWindowTitle(title);
	ui.labelMSG->setAlignment(Qt::AlignCenter);	// ���ö��뷽ʽ
	ui.labelMSG->setText(msg);
}