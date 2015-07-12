/*
 *
 * Copyright (C) 2015  Miroslav Krajicek (https://github.com/kaajo).
 * All Rights Reserved.
 *
 * This file is part of WebCamCap.
 *
 * WebCamCap is free software: you can redistribute it and/or modify
 * it under the terms of the GNU LGPL version 3 as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * WebCamCap is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU LGPL version 3
 * along with WebCamCap. If not, see <http://www.gnu.org/licenses/lgpl-3.0.txt>.
 *
 */

#ifndef IVIRTUALROOM_H
#define IVIRTUALROOM_H

#include "webcamcapinterfaces_global.h"

#include "iserver.h"
#include "animation.h"
#include "roomsettings.h"
#include "icameratopology.h"

#include <QLinkedList>
#include <QVariantMap>
#include <QObject>

class WEBCAMCAPINTERFACESSHARED_EXPORT IVirtualRoom : public QObject
{
    Q_OBJECT

protected:
    RoomSettings *m_settings = nullptr;
    ICameraTopology *m_cameraTopology = nullptr;
    QLinkedList<Animation*> m_animations;

public:
    explicit IVirtualRoom(RoomSettings *settings, QObject *parent = 0);
    virtual ~IVirtualRoom();

    RoomSettings *settings();
    void setSettings(RoomSettings *settings);

    ICameraTopology *cameraTopology() const;

    virtual QVariantMap toVariantMap() = 0;
    virtual void fromVariantMap(QVariantMap varMap) = 0;

signals:
    void animationRecorded(Animation *animation);

public slots:

private:

private slots:
    virtual void settingsChange(RoomSettings::RoomSettingsType type) = 0;
};

#endif // IVIRTUALROOM_H
