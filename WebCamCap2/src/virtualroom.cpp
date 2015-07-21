#include "virtualroom.h"

#include "polygoncameratopology.h"

VirtualRoom::VirtualRoom(QVariantMap varMap, QObject *parent) :
    IVirtualRoom(parent)
{
    fromVariantMap(varMap);
}

VirtualRoom::VirtualRoom(RoomSettings *settings, QObject *parent) :
    IVirtualRoom(parent)
{
    initProject(settings);
}

VirtualRoom::~VirtualRoom()
{

}

void VirtualRoom::recordScene(bool record)
{
    m_cameraTopology->record(record);
}

void VirtualRoom::recordAnimation(bool record)
{
    if(! m_settings->recordScene())
    {
        return;
    }

    if(record)
    {
        Animation *anim = new Animation();
        connect(m_cameraTopology, &ICameraTopology::frameReady, anim, &Animation::addFrame);

        m_animations.push_back(anim);
    }
    else
    {
        m_cameraTopology->disconnect(m_animations.last(), 0);

        emit animationRecorded(m_animations.last());
    }
}

void VirtualRoom::initProject(RoomSettings *settings)
{
    setSettings(settings);

    m_cameraTopology = new PolygonCameraTopology(settings->roomDimensions(), settings->maxError());
}

void VirtualRoom::settingsChange(RoomSettings::RoomSettingsType type)
{
    switch (type) {
    case RoomSettings::RoomSettingsType::RECORD:
        recordScene(m_settings->recordScene());
        break;
    case RoomSettings::RoomSettingsType::RECORDANIMATION:
        recordAnimation(m_settings->recordAnimation());
        break;
    case RoomSettings::RoomSettingsType::DIMENSIONS:
        m_cameraTopology->setRoomDimensions(m_settings->roomDimensions());
        break;
    case RoomSettings::RoomSettingsType::MAXERROR:
        m_cameraTopology->setMaxError(m_settings->maxError());
    default:
        break;
    }
}

//#########################################################

const QString settingsKey("settings");
const QString topologyKey("topology");

QVariantMap VirtualRoom::toVariantMap()
{
    QVariantMap retVal;

    retVal[settingsKey] = m_settings->toVariantMap();
    retVal[topologyKey] = m_cameraTopology->toVariantMap();

    return retVal;
}

bool VirtualRoom::fromVariantMap(QVariantMap varMap)
{
    //init settings and topology
    auto settings = new RoomSettings();

    if(! settings->fromVariantMap(varMap[settingsKey].toMap()))
    {
        qDebug() << "wrong RoomSettings variantMap";

        return false;
    }

    initProject(settings);

    if(! m_cameraTopology->fromVariantMap(varMap[topologyKey].toMap()))
    {
        qDebug() << "wrong PolygonCameraTopology variantMap";

        return false;
    }

    return true;
}
