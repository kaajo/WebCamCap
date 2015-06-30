#include "wccmainwindow.h"

#include <QtWidgets/QApplication>

#include "line.h"

int main(int argc, char *argv[])
{
    qRegisterMetaType<QVector<Line>>("QVector<Line>");
    qRegisterMetaType<Line>("Line");

    QApplication a(argc, argv);
    WccMainWindow w;
    w.show();

    return a.exec();
}
