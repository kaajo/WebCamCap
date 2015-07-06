/*
 *
 * Copyright (C) 2014  Miroslav Krajicek (https://github.com/kaajo) . All Rights Reserved.
 *
 * This file is part of WccAPI.
 *
 * WccAPI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU LGPL version 3 as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * WccAPI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU  LGPL version 3
 * along with WccAPI.  If not, see <http://www.gnu.org/licenses/lgpl-3.0.txt>.
 *
 */

#include "myfifo.h"

#include <string>
#include <sstream>

#include <QDebug>
#include <QDataStream>

MyFifo::MyFifo()
{
    m_socket = new QLocalSocket();
    setupSocket();

    QObject::connect(m_socket, SIGNAL(readyRead()), this, SLOT(handleMessageFromSocket()));
    QObject::connect(m_socket, SIGNAL(disconnected()), this , SLOT(disconnected()));
    QObject::connect(m_socket, SIGNAL(connected()), this, SLOT(connected()));
}

MyFifo::~MyFifo()
{
    m_socket->abort();
    delete m_socket;
}

void MyFifo::tryConnectToServer()
{
    if(!m_haveConnection)
    {
        qDebug() << "trying to connect to server";
        m_socket->setServerName("webcamcap");
        m_socket->open(QIODevice::ReadOnly);
    }
}

void MyFifo::disconnectServer()
{
    if(m_haveConnection)
    {
        m_socket->disconnectFromServer();
        m_haveConnection = false;
    }
}

bool MyFifo::isConnected()
{
    return m_haveConnection;
}

void MyFifo::handleMessageFromSocket()
{
    QDataStream in(m_socket);
    in.setVersion(QDataStream::Qt_4_0);

    if (m_blockSize == 0)
    {
        if (m_socket->bytesAvailable() < (int)sizeof(quint16))
            return;

        in >> m_blockSize;
    }

    if (in.atEnd())
    {
        qDebug() << "empty message";
        return;
    }

    QString qmsg;
    in >> qmsg;

    std::string msg;
    msg = qmsg.toStdString();

    QVector<Point> pts = messageToPoint(msg);
    emit pointsReady(pts);


    m_blockSize = 0;
}

void MyFifo::disconnected()
{
    m_haveConnection = false;
    qDebug() << "disconnected";
    m_socket->disconnectFromServer();
    emit disconnectedServer();
}

void MyFifo::connected()
{
    if(!m_haveConnection)
    {
        m_haveConnection = true;
        qDebug() << "connected";
        emit connectedServer();
    }
}

void MyFifo::setupSocket()
{
    m_socket->setReadBufferSize(1024);
}


QVector<Point> MyFifo::messageToPoint(std::string msg)
{
    std::stringstream split(msg);
    size_t NumOfPoints = 0;

    split >> NumOfPoints;

    int id;
    float x, y, z;
    QVector<Point> ret;

    for(size_t i = 0; i < NumOfPoints; i++)
    {
        split >> id;
        split >> x;
        split >> y;
        split >> z;

        Point point(id, glm::vec3(x,y,z));

        ret.push_back(point);
    }
    return ret;
}
