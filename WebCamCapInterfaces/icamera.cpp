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

std::shared_ptr<CameraSettings> ICamera::settings() const
{
    return m_settings;
}

void ICamera::setSettings(std::shared_ptr<CameraSettings> settings)
{
    m_settings = settings;

    connect(m_settings.get(), &CameraSettings::changed, this, &ICamera::settingsChanged);

    settingsChanged(CameraSettings::CameraSettingsType::ALL);
}
