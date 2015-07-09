#include "roomsettings.h"


RoomSettings::RoomSettings(QObject *parent) :
    QObject(parent)
{
    connect(this, &RoomSettings::changed, this, &RoomSettings::setSave);
}

QString RoomSettings::name() const
{
    return m_name;
}

void RoomSettings::setName(const QString &name)
{
    m_name = name;

    emit changed(RoomSettingsType::NAME);
}

QVector3D RoomSettings::roomDimensions() const
{
    return m_roomDimensions;
}

void RoomSettings::setRoomDimensions(const QVector3D &roomDimensions)
{
    m_roomDimensions = roomDimensions;

    emit changed(RoomSettingsType::DIMENSIONS);
}

double RoomSettings::maxError() const
{
    return m_maxError;
}

void RoomSettings::setMaxError(double maxError)
{
    m_maxError = maxError;

    emit changed(RoomSettingsType::MAXERROR);
}

bool RoomSettings::recordScene() const
{
    return m_recordScene;
}

void RoomSettings::setRecordScene(bool recordScene)
{
    m_recordScene = recordScene;

    emit changed(RoomSettingsType::RECORD);
}

bool RoomSettings::recordAnimation() const
{
    return m_recordAnimation;
}

void RoomSettings::setRecordAnimation(bool recordAnimation)
{
    m_recordAnimation = recordAnimation;

    emit changed(RoomSettingsType::RECORDANIMATION);
}

bool RoomSettings::saved() const
{
    return m_saved;
}

void RoomSettings::save()
{
    m_saved = true;
}

void RoomSettings::setSave(RoomSettings::RoomSettingsType /*type*/)
{
    m_saved = false;
}
