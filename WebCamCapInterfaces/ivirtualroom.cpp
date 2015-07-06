#include "ivirtualroom.h"


IVirtualRoom::IVirtualRoom(QObject *parent) :
    QObject(parent)
{
}

IVirtualRoom::~IVirtualRoom()
{
    
}

RoomSettings *IVirtualRoom::settings()
{
    return m_settings;
}

void IVirtualRoom::setSettings(RoomSettings *settings)
{
    if(m_settings)
    {
        delete m_settings;
    }

    m_settings = settings;

    connect(m_settings, &RoomSettings::changed, this, &IVirtualRoom::settingsChange);

    settingsChange(RoomSettings::RoomSettingsType::ALL);
}

ICameraTopology *IVirtualRoom::cameraTopology() const
{
    return m_cameraTopology;
}
