#include "gui/wccmainwindow.h"

#include <QtWidgets/QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WccMainWindow w;

    qRegisterMetaType<QVector<Line>>("QVector<Line>");
    qRegisterMetaType<Line>("Line");
    qRegisterMetaType<cv::UMat>("cv::UMat");

    qDebug() << "WebCamCap2 is running under Qt version: " << qVersion();

    w.show();
    return a.exec();
}
