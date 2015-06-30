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

QVector2D CameraSettings::resolution() const
{
    return m_resolution;
}

void CameraSettings::setResolution(const QVector2D &resolution)
{
    m_resolution = resolution;

    emit changed(CameraSettingsType::RESOLUTION);
}

void CameraSettings::save()
{
    m_saved = true;
}

QVector3D CameraSettings::pixelLineDirectionVector(int x, int y)
{
    return m_pixelLines[x][y];
}

bool CameraSettings::turnedOn() const
{
    return m_turnedOn;
}

void CameraSettings::setTurnedOn(bool turnedOn)
{
    m_turnedOn = turnedOn;

    emit changed(CameraSettingsType::TURNEDON);
}

bool CameraSettings::UseBackgroundSub() const
{
    return useBackgroundSub;
}

void CameraSettings::setUseBackgroundSub(bool value)
{
    useBackgroundSub = value;

    emit changed(CameraSettingsType::USEBACKGROUNDSUBSTRACTOR);
}

bool CameraSettings::showWindow() const
{
    return m_showWindow;
}

void CameraSettings::setShowWindow(bool showWindow)
{
    m_showWindow = showWindow;

    emit changed(CameraSettingsType::SHOWIMAGEGUI);
}

bool CameraSettings::getROI() const
{
    return ROI;
}

void CameraSettings::setROI(bool value)
{
    ROI = value;

    emit changed(CameraSettingsType::ROI);
}

cv::Mat CameraSettings::getROIMask() const
{
    return ROIMask;
}

void CameraSettings::setROIMask(const cv::Mat &value)
{
    ROIMask = value;

    emit changed(CameraSettingsType::ROIMASK);
}

QString CameraSettings::name() const
{
    return m_name;
}

void CameraSettings::setName(const QString &name)
{
    m_name = name;

    emit changed(CameraSettingsType::NAME);
}

int CameraSettings::videoUsbId() const
{
    return m_videoUsbId;
}

void CameraSettings::setVideoUsbId(int videoUsbId)
{
    m_videoUsbId = videoUsbId;

    emit changed(CameraSettingsType::VIDEOUSBID);
}

float CameraSettings::diagonalFov() const
{
    return m_diagonalFov;
}

void CameraSettings::setDiagonalFov(float diagonalFov)
{
    m_diagonalFov = diagonalFov;

    emit changed(CameraSettingsType::FOV);
}

QVector3D CameraSettings::globalPosition() const
{
    return m_globalPosition;
}

void CameraSettings::setGlobalPosition(const QVector3D &globalPosition)
{
    m_globalPosition = globalPosition;

    emit changed(CameraSettingsType::POSITION);
}

cv::UMat CameraSettings::distortionCoeffs() const
{
    return m_distortionCoeffs;
}

void CameraSettings::setDistortionCoeffs(const cv::UMat &distortionCoeffs)
{
    m_distortionCoeffs = distortionCoeffs;

    emit changed(CameraSettingsType::DISTORTION);
}

QVector3D CameraSettings::roomDimensions() const
{
    return m_roomDimensions;
}

void CameraSettings::setRoomDimensions(const QVector3D &roomDimensions)
{
    m_roomDimensions = roomDimensions;

    ///compute
}

void CameraSettings::setSave(CameraSettings::CameraSettingsType /*type*/)
{
    m_saved = false;
}

void CameraSettings::computeParameters()
{
    ///todo
}
