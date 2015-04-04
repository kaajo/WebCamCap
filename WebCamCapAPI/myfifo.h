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
 * @class MyFifo myfifo.h
 * @brief Small class used to transfer information from main WebCamCap application to ControlPanel.
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
     * @brief Tries to connect MyFifo to server.
     */
    void tryConnectToServer();

    /**
     * @brief Disconnect MyFifo from server.
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
     * @brief This signal is emitted when MyFifo is disconnected from server.
     */
    void disconnectedServer();

    /**
     * @brief This signal is emitted when MyFifo is connected to server.
     */
    void connectedServer();

    /**
     * @brief Emitted every time when points are received from WebCamCap. This signal is emitted if there were no points in the frame.
     * @param pts Point received from WebCamCap
     */
    void pointsReady(QVector<Point> pts);

private:
    void setupSocket();
    QVector<Point> messageToPoint(std::string msg);
};


#endif // MYFIFO_H
