#include "ivirtualroom.h"

IVirtualRoom::IVirtualRoom(QObject *parent) :
    QObject(parent)
{
}

RoomSettings &IVirtualRoom::settings()
{
    return m_settings;
}
