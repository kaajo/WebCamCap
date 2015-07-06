#ifndef VIRTUALROOM_H
#define VIRTUALROOM_H

#include "ivirtualroom.h"

class VirtualRoom : public IVirtualRoom
{
    Q_OBJECT
public:
    explicit VirtualRoom(QObject *parent = 0);
    virtual ~VirtualRoom();

    virtual void recordScene(bool record) override;
    virtual void recordAnimation(bool record) override;

    virtual QVariantMap toVariantMap() override;
    virtual void fromVariantMap(QVariantMap varMap) override;
signals:

public slots:

private:
    virtual void settingsChange(RoomSettings::RoomSettingsType type) override;

};

#endif // VIRTUALROOM_H
