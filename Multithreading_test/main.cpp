#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName(APP_PRODUCT);
    MainWindow w;
    w.show();
    return a.exec();
}
