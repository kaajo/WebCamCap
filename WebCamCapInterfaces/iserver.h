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

#ifndef ISERVER_H
#define ISERVER_H

#include "webcamcapinterfaces_global.h"

#include <QObject>
#include <QVariantMap>

class WEBCAMCAPINTERFACESSHARED_EXPORT IServer : public QObject
{
    Q_OBJECT

protected:
    bool m_enabled = false;
    QString m_name;

public:
    explicit IServer(QObject *parent = 0);
    virtual ~IServer();

    virtual bool setEnabled(bool enabled) = 0;
    virtual void sendMesage(QVariantMap message) = 0;

    QString name() const;
    void setName(const QString &name);

    virtual QVariantMap toVariantMap() const = 0;
    virtual void fromVariantMap(QVariantMap varMap) = 0;

signals:

public slots:
    virtual void newConnection() = 0;
    virtual void socketDisconnected() = 0;
};

#endif // ISERVER_H
