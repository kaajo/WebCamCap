/*
 *
 * Copyright (C) 2014  Miroslav Krajicek, Faculty of Informatics Masaryk University (https://github.com/kaajo).
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

#include "openglwindow.h"
#include <iostream>
#include <GL/glu.h>
#include <GL/glut.h>

#include <QThread>

using glm::vec2;
using glm::vec3;


bool OpenGLWindow::getTwoDimensions() const
{
    return twoDimensions;
}

void OpenGLWindow::setTwoDimensions(bool value)
{
    twoDimensions = value;
}
OpenGLWindow::OpenGLWindow(QWidget *parent) : QGLWidget(parent)
{
    zoom = 1.0f;
    twoDimensions = false;
    mdrawJoints = mdrawLines = mdrawBones = true;
    roomDims = vec3(100.0f, 100.0f, 100.0f);
    camRot = vec3(0.0f, 0.0f, 0.0f);

    //mouse
    currentMousePos = lastMousePos = vec2(0.0f, 0.0f);
    leftButton = false;

    //QImage temp(QDir::currentPath() + "/Pictures/checkboard_texture.jpg");

    //texture = QGLWidget::convertToGLFormat(temp);

    generateNewRandomColor();
}

void OpenGLWindow::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glShadeModel(GL_SMOOTH);
    glRenderMode(GL_RENDER);

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);

    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);

/*
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texture.width(), texture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.bits() );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glDisable(GL_TEXTURE_2D);
*/
}

void OpenGLWindow::paintGL()
{
    glClearColor(0.15f, 0.15f, 0.15f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    gluLookAt(
                -roomDims.x*zoom, 200*zoom, roomDims.y*zoom,
                 roomDims.x/2.0f, roomDims.z/2.0f , -roomDims.y/2.0f,
                 0, 1, 0
                    );

    glTranslatef(roomDims.x/2.0f, roomDims.z/2.0f, -roomDims.y/2.0f);
    glRotatef(camRot.x, 1, 0, 0);
    glRotatef(camRot.y, 0, 1, 0);
    glTranslatef(-roomDims.x/2.0f, -roomDims.z/2.0f, roomDims.y/2.0f);

    if(mdrawJoints)
    {
        drawJoints();
    }

    if(mdrawLines)
    {
        drawLines();
    }

    drawFloor();
}

void OpenGLWindow::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float) w/ (float) h, 0.1, 10000);

    glMatrixMode(GL_MODELVIEW);
}

void OpenGLWindow::setRoomDims(vec3 dims)
{
    roomDims = dims;
    updateGL();
}

void OpenGLWindow::setFrame(std::vector<Point> pts, QVector<QVector<Line> > lns)
{
    joints = pts;
    lines = lns;

    updateGL();
}

void OpenGLWindow::wheelEvent(QWheelEvent *event)
{
    int numDegrees = -event->delta() / 8;
    int numSteps = numDegrees / 15;

    zoom += 0.1*numSteps;

    if(zoom < 0.5)
    {
        zoom = 0.5;
    }
    else if(zoom > 5.0)
    {
        zoom = 5.0;
    }

    updateGL();
    event->accept();
}

void OpenGLWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        QCursor c = cursor();
        c.setShape(Qt::DragMoveCursor);
        setCursor(c);

        leftButton = true;
        currentMousePos.x = event->x();
        currentMousePos.y = event->y();
    }

    event->accept();
}

void OpenGLWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        QCursor c = cursor();
        c.setShape(Qt::OpenHandCursor);
        setCursor(c);

        leftButton = false;

    }

    event->accept();
}

void OpenGLWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(leftButton)
    {
        lastMousePos = currentMousePos;

        currentMousePos.x = event->x();
        currentMousePos.y = event->y();

        camRot.y += (currentMousePos.x - lastMousePos.x);
        camRot.z = camRot.x += (currentMousePos.y - lastMousePos.y);

        limitsCamRot();
    }

    updateGL();
    event->accept();
}

void OpenGLWindow::limitsCamRot()
{
    if(camRot.y >= 360.0f)
    {
        camRot.y -= 360.0f;
    }

    if(camRot.y <= 360.0f)
    {
        camRot.y += 360.0f;
    }

    if(camRot.x > 30.0f)
    {
        camRot.x  = 30.0f;
    }

    if(camRot.x < -30.0f)
    {
        camRot.x  = -30.0f;
    }
}

void OpenGLWindow::countView()
{

}

void OpenGLWindow::drawLine(const Line &l)
{
    glBegin(GL_LINES);
    glVertex3f(l.m_position.x, l.m_position.z, -l.m_position.y);
    glVertex3f(l.m_position.x+2*l.m_directionVector.x, l.m_position.z+2*l.m_directionVector.z, -1*(l.m_position.y+2*l.m_directionVector.y));
    glEnd();
}

void OpenGLWindow::drawLines()
{
    glColor3f(1.0f,0.5f,0.0f);

    for(size_t i = 0; i < lines.size(); i++)
    {
        for(size_t j = 0; j < lines[i].size(); j++)
        {
            drawLine(lines.at(i).at(j));
        }
    }
}

void OpenGLWindow::drawJoints()
{
    glPushMatrix();
    for(size_t i = 0; i < joints.size(); i++)
    {
        //std::cout << joints[i].ID << " ";
        glColor3ub(randomColors[joints[i].m_id][0],randomColors[joints[i].m_id][1],randomColors[joints[i].m_id][2]);

        if(twoDimensions)
        {
            glTranslatef(joints[i].m_position.x*100, joints[i].m_position.z, -joints[i].m_position.y*100);
            glutSolidSphere(3, 8, 8);
            glTranslatef(-joints[i].m_position.x*100, -joints[i].m_position.z, joints[i].m_position.y*100);
        }
        else
        {
            glTranslatef(joints[i].m_position.x, joints[i].m_position.z, -joints[i].m_position.y);
            glutSolidSphere(3, 8, 8);
            glTranslatef(-joints[i].m_position.x, -joints[i].m_position.z, joints[i].m_position.y);
        }

    }
    //std::cout << std::endl;
    glPopMatrix();
}

void OpenGLWindow::drawFloor()
{
    glColor3f(0.2, 0.1, 0.7);

    glBegin(GL_QUADS);
    glTexCoord2f(0,0); glVertex3d(0, -1, 0);
    glTexCoord2f(1,0); glVertex3d(roomDims.x, -1, 0);
    glTexCoord2f(1,1); glVertex3d(roomDims.x, -1, -roomDims.y);
    glTexCoord2f(0,1); glVertex3d(0, -1, -roomDims.y);
    glEnd();
}

void OpenGLWindow::DefaultView()
{

}


void OpenGLWindow::loadTexture()
{

}

void OpenGLWindow::generateNewRandomColor()
{
    for(size_t i = 0; i < 100; i++)
    {
        std::vector<int> ints;
        ints.push_back(qrand()%255);
        QThread::msleep(2);
        ints.push_back(qrand()%255);
        QThread::msleep(2);
        ints.push_back(qrand()%255);

        randomColors.push_back(ints);
    }

}
