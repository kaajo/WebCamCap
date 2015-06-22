#ifndef IVIRTUALROOM_H
#define IVIRTUALROOM_H

#include "roomsettings.h"

#include <QVariantMap>
#include <QObject>

class IVirtualRoom : public QObject
{
    Q_OBJECT

protected:
    RoomSettings m_settings;

public:
    explicit IVirtualRoom(QObject *parent = 0);
    virtual ~IVirtualRoom();

    virtual void fromVariantMap(QVariantMap map) = 0;
    virtual QVariantMap toVariantMap() = 0;

    RoomSettings &settings();

signals:
    void startRecording();
    void stopRecording();
    void startWork2D();


public slots:

};

#endif // IVIRTUALROOM_H
