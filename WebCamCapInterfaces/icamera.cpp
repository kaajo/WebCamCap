#include "icamera.h"


void ICamera::setWaitCondition(QWaitCondition *waitCondition)
{
    m_waitCondition = waitCondition;
}
ICamera::ICamera(CameraSettings *settings, QObject *parent) : QObject(parent)
{
    setSettings(settings);
}

ICamera::~ICamera()
{
    emit finished();
}

CameraSettings *ICamera::settings() const
{
    return m_settings;
}

void ICamera::setSettings(CameraSettings *settings)
{
    if(m_settings)
    {
        delete m_settings;
    }

    m_settings = settings;

    connect(m_settings, &CameraSettings::changed, this, &ICamera::settingsChanged);
}
