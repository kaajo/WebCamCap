#include "virtualroom.h"

#include "polygoncameratopology.h"

VirtualRoom::VirtualRoom(RoomSettings *settings, QObject *parent) :
    IVirtualRoom(settings, parent)
{
    setSettings(settings);

    m_cameraTopology = new PolygonCameraTopology();
}

VirtualRoom::~VirtualRoom()
{

}

void VirtualRoom::recordScene(bool record)
{

}

void VirtualRoom::recordAnimation(bool record)
{

}

QVariantMap VirtualRoom::toVariantMap()
{

}

void VirtualRoom::fromVariantMap(QVariantMap varMap)
{

}

void VirtualRoom::settingsChange(RoomSettings::RoomSettingsType type)
{

}
