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

#include "controlpanel.h"
#include "ui_controlpanel.h"

#include <QDebug>

ControlPanel::ControlPanel(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ControlPanel)
{
    ui->setupUi(this);

    setWindowTitle("Control Panel");

    m_movementTimer = new QTime();
    m_fifo = new MyFifo();

    connect(m_fifo, SIGNAL(disconnectedServer()), this, SLOT(disconnected()));
    connect(m_fifo, SIGNAL(connectedServer()), this, SLOT(connected()));
    connect(m_fifo, &MyFifo::frameReady, this, &ControlPanel::handleFrame);

    m_sensitivity = ui->sensitivity->value();
    minSpeed = ui->speed->value()*0.01f;
    minSize = ui->size->value()*0.01f;
    minAcceleration = ui->acceleration->value()*0.01f;
    m_nextMovementCaptureTime = ui->timeout->value();

    if(ui->movement->checkState() == Qt::Checked)
    {
        m_sendMovement = true;
    }
    else
    {
        m_sendMovement = false;
        ui->acceleration->setDisabled(true);
        ui->speed->setDisabled(true);
        ui->size->setDisabled(true);
        ui->timeout->setDisabled(true);
    }

    if(ui->positions->checkState() == Qt::Checked)
    {
        m_sendPositions = true;
    }
    else
    {
        m_sendPositions = false;
    }

    m_reconnectTimer = new QTimer();
    m_reconnectTimer->setInterval(5000);
    connect(m_reconnectTimer, SIGNAL(timeout()), this, SLOT(tryConnect()));
    m_reconnectTimer->start();
}

ControlPanel::~ControlPanel()
{
    if(m_fifo->isConnected())
    {
        m_fifo->disconnectServer();
    }

    delete m_reconnectTimer;
    delete m_movementTimer;
    delete m_fifo;
    delete ui;
}

void ControlPanel::handleFrame(Frame frame)
{
    makeDiff(frame.markers());

    if(m_sendPositions)
    {
        handlePositions();
    }

    if(m_sendMovement)
    {
        handleMovements();
    }
}


void ControlPanel::handlePositions()
{

        QMap<size_t, Marker> sendPts = points;

/*        if(sendPts.size() > pts.size())
        {
            sendPts.erase(sendPts.begin()+pts.size(), sendPts.end());
        }
*/
        for(auto it : sendPts)
        {
            auto position = it.position();

            if(position.x() > 1.0f)
            {
                position.setX(1.0f);
            }
            else if(position.x() < 0.0f)
            {
                position.setX(0.0f);
            }

            if(position.y() > 1.0f)
            {
                position.setY(1.0f);
            }
            else if(position.y() < 0.0f)
            {
                position.setY(0.0f);
            }

            if(position.z() > 1.0f)
            {
                position.setZ(1.0f);
            }
            else if(position.z() < 0.0f)
            {
                position.setZ(0.0f);
            }
        }

    emit pointsReady(sendPts.values().toVector());
}

void ControlPanel::handleMovements()
{
    if(m_movementTimer->elapsed() < m_nextMovementCaptureTime)
    {
        return;
    }

    for(int i = 0; i < points.size(); i++)
    {

        difTemp = sqrt(diff[i].position().x() * diff[i].position().x() + diff[i].position().y()*diff[i].position().y());
        if(difTemp > minSpeed)
        {
            if(m_minSizeOfMovement == 0.0f)
            {
                m_startMovementPoint = points[i].position();
            }

            m_minSizeOfMovement+=difTemp;
            if(m_minSizeOfMovement >= minSize)
            {
                m_endMovementPoint = points[i].position();
                emit movement(Movement(glm::vec3(m_endMovementPoint.x()- m_startMovementPoint.x(), m_endMovementPoint.y()- m_startMovementPoint.y(), 0.0f), m_minSizeOfMovement), i);
                qDebug() << "MOVE";
                m_minSizeOfMovement = 0.0f;
                m_movementTimer->restart();
            }
        }
        else
        {
            m_minSizeOfMovement = 0.0f;
        }
    }
}

void ControlPanel::tryConnect()
{
    m_fifo->tryConnectToServer();
}

void ControlPanel::connected()
{
    m_reconnectTimer->stop();
    m_movementTimer->start();

    auto palete = ui->onlineStatusLabel->palette();
    palete.setColor(QPalette::WindowText, Qt::GlobalColor::green);
    ui->onlineStatusLabel->setPalette(palete);
    ui->onlineStatusLabel->setText("Online");
}

void ControlPanel::disconnected()
{
    m_reconnectTimer->start();

    auto palete = ui->onlineStatusLabel->palette();
    palete.setColor(QPalette::WindowText, Qt::GlobalColor::red);
    ui->onlineStatusLabel->setPalette(palete);
    ui->onlineStatusLabel->setText("Offline");
}

void ControlPanel::on_positions_stateChanged(int arg1)
{
    if(arg1 == 0)
    {
        m_sendPositions = false;
    }
    else
    {
        m_sendPositions = true;
    }
}

void ControlPanel::on_movement_stateChanged(int arg1)
{
    if(arg1 == 0)
    {
        m_sendMovement = false;
    }
    else
    {
        m_sendMovement = true;
    }

}

void ControlPanel::on_sensitivity_valueChanged(double arg1)
{
     m_sensitivity = arg1;
}

void ControlPanel::on_speed_valueChanged(double arg1)
{
    minSpeed = arg1*0.01f;
}

void ControlPanel::on_acceleration_valueChanged(double arg1)
{
    minAcceleration = arg1*0.01f;
}

void ControlPanel::on_size_valueChanged(double arg1)
{
    minSize = arg1*0.01;
}

void ControlPanel::on_timeout_valueChanged(int arg1)
{
    m_nextMovementCaptureTime = arg1;
}

void ControlPanel::makeDiff(QVector<Marker> pts)
{
    lastDiff = diff;
    lastPoints = points;

    diff.clear();
    points.clear();

    for(Marker &pnt : pts)
    {
        points[pnt.id()] = pnt;
    }

    for(size_t &index : points.keys())
    {
        auto it = lastPoints.find(index);

        if(it != lastPoints.end())
        {
            diff[index] = (points[index] - it.value())*m_sensitivity;
        }
    }

    for(size_t &index : diff.keys())
    {
        auto it = lastDiff.find(index);

        if(it != lastDiff.end())
        {
            acceleration[index] = diff[index] - it.value();
        }
    }
}
