/*
 *
 * Copyright (C) 2014  Miroslav Krajicek (https://github.com/kaajo) . All Rights Reserved.
 *
 * This file is part of WccPong.
 *
 * WccPong is free software: you can redistribute it and/or modify
 * it under the terms of the GNU LGPL version 3 as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * WccPong is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU  LGPL version 3
 * along with WccPong.  If not, see <http://www.gnu.org/licenses/lgpl-3.0.txt>.
 *
 */

#ifndef MYFIFO_H
#define MYFIFO_H

#include "point.h"

#include <glm/glm.hpp>

#include <QVector>
#include <QLocalSocket>
#include <QObject>

/**
 * @author Miroslav Krajicek <409917@mail.muni.cz>
 * @class Movement
 * @brief Small class used to keep information about direction and size of movement.
 * @date 2015
 */

class MyFifo : public QObject
{
    Q_OBJECT

    bool m_haveConnection = false;

    QLocalSocket *m_socket = nullptr;
    quint16 m_blockSize = 0;
public:

    MyFifo();
    ~MyFifo();

    /**
     * @brief tryConnectToServer
     */

    void tryConnectToServer();

    /**
     * @brief disconnectServer
     */

    void disconnectServer();

    /**
     * @brief isConnected
     * @return true if connection between MyFifo and WebCamCap application exists otherwise false.
     */

    bool isConnected();

private slots:

    void disconnected();
    void connected();
    void handleMessageFromSocket();

signals:

    /**
     * @brief disconnectedServer
     */
    void disconnectedServer();

    /**
     * @brief connectedServer
     */
    void connectedServer();

    /**
     * @brief pointsReady
     * @param pts
     */
    void pointsReady(QVector<Point> pts);

private:
    void setupSocket();
    QVector<Point> messageToPoint(std::string msg);
};


#endif // MYFIFO_H
