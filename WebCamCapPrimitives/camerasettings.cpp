#include "camerasettings.h"

CameraSettings::CameraSettings(QString name, int videoUsbId , float diagonalFov ,
                               QVector3D globalPosition , QVector3D roomDimensions,  QObject *parent)
                : QObject(parent)
{
    m_name = name;
    m_videoUsbId = videoUsbId;
    m_diagonalFov = diagonalFov;
    m_globalPosition = globalPosition;
    m_roomDimensions = roomDimensions;

    computeParameters();
}

void CameraSettings::computeParameters()
{
    ///todo

    emit changed();
}
