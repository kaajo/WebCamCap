#include "wccmainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WccMainWindow w;
    w.show();

    return a.exec();
}
