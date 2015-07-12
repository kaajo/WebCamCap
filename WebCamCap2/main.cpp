#include "gui/wccmainwindow.h"

#include <QtWidgets/QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WccMainWindow w;

    qRegisterMetaType<QVector<Line>>("QVector<Line>");
    qRegisterMetaType<Line>("Line");
    qRegisterMetaType<Frame>("Frame");
    qRegisterMetaType<Animation*>("Animation*");
    qRegisterMetaType<cv::UMat>("cv::UMat");
    qRegisterMetaType<QVector<Line>>("QVector<Line>");

    qDebug() << "WebCamCap2 is running under Qt version: " << qVersion();

    w.show();
    return a.exec();
}
