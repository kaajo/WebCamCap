#include "icamera.h"


CameraSettings *ICamera::settings() const
{
    return m_settings;
}

void ICamera::setSettings(CameraSettings *settings)
{
    m_settings = settings;
}

ICamera::ICamera(QObject *parent) : QObject(parent)
{
    connect(m_settings, &CameraSettings::changed, this, &ICamera::settingsChanged);
}
