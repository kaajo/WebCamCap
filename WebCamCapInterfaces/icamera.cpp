#include "icamera.h"

ICamera::ICamera( QObject *parent) : QObject(parent)
{
}

ICamera::~ICamera()
{
    emit finished();
}

void ICamera::setWaitCondition(QWaitCondition *waitCondition)
{
    m_waitCondition = waitCondition;
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

    settingsChanged(CameraSettings::CameraSettingsType::ALL);
}
