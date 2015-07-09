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

#ifndef LOCALSERVER_H
#define LOCALSERVER_H

#include "iserver.h"

#include <QtNetwork/QLocalServer>
#include <QtNetwork/QLocalSocket>
#include <QLinkedList>

class LocalServer : public IServer
{
    Q_OBJECT

    QLocalServer *m_server = nullptr;
    QLinkedList<QLocalSocket*> m_sockets;

public:
    explicit LocalServer(QString name = "webcamcap", QObject *parent = 0);
    virtual ~LocalServer();

    virtual bool setEnabled(bool enabled) override;
    virtual void sendMesage(QVariantMap &message) override;

    virtual QVariantMap toVariantMap() const override;
    virtual void fromVariantMap(QVariantMap varMap) override;
private:
    virtual void newConnection() override;
    virtual void socketDisconnected() override;
};

#endif // LOCALSERVER_H
