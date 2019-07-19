#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

MainWindow *g_mw = nullptr;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setQuitOnLastWindowClosed(false);
    MainWindow w;
    g_mw = &w;
#ifdef QT_DEBUG
    w.show();
#endif
    return QApplication::exec();
}
