#include "roomsettings.h"

#include <QString>
#include <QVariantMap>

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

    if(! recordScene)
    {
        setRecordAnimation(false);
    }

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

//########################################################################

const QString nameKey("name");
const QString dimensionsKey("dimensions");
const QString maxErrorKey("maxError");

QVariantMap RoomSettings::toVariantMap()
{
    QVariantMap varMap;

    varMap[nameKey] = m_name;
    varMap[dimensionsKey] = m_roomDimensions;
    varMap[maxErrorKey] = m_maxError;

    emit changed(RoomSettingsType::ALL);

    return varMap;
}

bool RoomSettings::fromVariantMap(QVariantMap map)
{
    if(! map.contains(nameKey) || ! map.contains(dimensionsKey) || ! map.contains(maxErrorKey))
    {
        return false;
    }

    m_name = map[nameKey].toString();
    m_roomDimensions = map[dimensionsKey].value<QVector3D>();
    m_maxError = map[maxErrorKey].toDouble();

    return true;
}
