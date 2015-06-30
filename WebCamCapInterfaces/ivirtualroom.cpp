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

bool IVirtualRoom::addServer(IServer *server)
{
    QString name = server->name();

    auto it = std::find_if(m_servers.begin(), m_servers.end(), [name](IServer *s){return s->name() == name;});

    if(it == m_servers.end())
    {
        m_servers.push_back(server);
        return true;
    }
    else
    {
        return false;
    }
}

bool IVirtualRoom::removeServer(QString name)
{
    auto it = std::find_if(m_servers.begin(), m_servers.end(), [name](IServer *s){return s->name() == name;});

    return true;
}

