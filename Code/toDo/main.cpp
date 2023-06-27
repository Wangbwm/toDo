#include "kmainwindow.h"
#include <QtWidgets/QApplication>
#include <QMetaType>

int main(int argc, char *argv[])
{
	if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
		QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // EnableHighDpiScaling
    QApplication a(argc, argv);
    KMainWindow w;
    //w.show();
    return a.exec();
}
