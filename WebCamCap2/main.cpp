#include "gui/wccmainwindow.h"

#include <QtWidgets/QApplication>
#include <QDebug>
#include <opencv2/core/ocl.hpp>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Kaajo-WebCamCap");
    QCoreApplication::setOrganizationDomain("https://github.com/kaajo/WebCamCap");
    QCoreApplication::setApplicationVersion("1.0");
    QCoreApplication::setApplicationName("WebCamCap");

    qRegisterMetaType<QVector<Line>>("QVector<Line>");
    qRegisterMetaType<Line>("Line");
    qRegisterMetaType<Frame>("Frame");
    qRegisterMetaType<Animation*>("Animation*");
    qRegisterMetaType<cv::UMat>("cv::UMat");
    qRegisterMetaType<cv::Mat>("cv::Mat");
    qRegisterMetaType<QVector<Line>>("QVector<Line>");
    qRegisterMetaType<CameraSettings::CameraSettingsType>("CameraSettings::CameraSettingsType");
    qRegisterMetaType<RoomSettings::RoomSettingsType>("RoomSettings::RoomSettingsType");

    qDebug() << "WebCamCap2 is running under Qt version: " << qVersion();
    qDebug() << "OpenCL availability: " << cv::ocl::haveOpenCL();

    WccMainWindow w;
    w.show();
    return a.exec();
}