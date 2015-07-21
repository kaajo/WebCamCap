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

#ifndef VIRTUALROOM_H
#define VIRTUALROOM_H

#include "animation.h"
#include "ivirtualroom.h"

class VirtualRoom : public IVirtualRoom
{
    Q_OBJECT

public:
    explicit VirtualRoom(QVariantMap varMap, QObject *parent = 0);
    VirtualRoom(RoomSettings* settings, QObject *parent = 0);
    virtual ~VirtualRoom();  

    virtual QVariantMap toVariantMap() override;
    virtual bool fromVariantMap(QVariantMap varMap) override;
signals:

public slots:

private:
    virtual void settingsChange(RoomSettings::RoomSettingsType type) override;
    void recordScene(bool record);
    void recordAnimation(bool record);

    inline void initProject(RoomSettings *settings);

private slots:

};

#endif // VIRTUALROOM_H
