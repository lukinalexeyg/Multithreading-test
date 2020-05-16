#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName(APP_PRODUCT);
    QCoreApplication::setApplicationVersion(APP_VERSION);
    MainWindow w;
    w.show();
    return a.exec();
}
