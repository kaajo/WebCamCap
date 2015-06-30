#ifndef IVIRTUALROOM_H
#define IVIRTUALROOM_H

#include "webcamcapinterfaces_global.h"

#include "iserver.h"
#include "animation.h"
#include "roomsettings.h"
#include "icameratopology.h"

#include <QVector>
#include <QVariantMap>
#include <QObject>

class WEBCAMCAPINTERFACESSHARED_EXPORT IVirtualRoom : public QObject
{
    Q_OBJECT

protected:
    RoomSettings *m_settings = nullptr;
    ICameraTopology *m_cameraTopology = nullptr;

    QVector<IServer*> m_servers;
    QVector<Animation> m_animations;

public:
    explicit IVirtualRoom(QObject *parent = 0);
    virtual ~IVirtualRoom();

    RoomSettings *settings();
    void setSettings(RoomSettings *settings);

    bool addServer(IServer *server);
    bool removeServer(QString name);

    virtual QVariantMap toVariantMap() = 0;
    virtual void fromVariantMap(QVariantMap varMap) = 0;
signals:
    void startRecording();
    void stopRecording();
    void startRecording2D();

public slots:

private slots:
    virtual void settingsChange(RoomSettings::RoomSettingsType type) = 0;
};

#endif // IVIRTUALROOM_H
