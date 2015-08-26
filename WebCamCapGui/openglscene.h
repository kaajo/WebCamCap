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

#ifndef OPENGLSCENE_H
#define OPENGLSCENE_H

#include "webcamcapgui_global.h"

#include "frame.h"
#include "camerasettings.h"

#include <QColor>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QOpenGLWidget>
#include <GL/glu.h>

#include <memory>

/**
 * @author Miroslav Krajicek
 * @brief The OpenGlScene class is widget for drawing #Frame or whole #Animation . It should be singleton.
 */
class WEBCAMCAPGUISHARED_EXPORT OpenGlScene : public QOpenGLWidget
{
    static OpenGlScene *m_scene;

    QVector3D m_roomDims = QVector3D(100,100,100);
    QVector<std::shared_ptr<CameraSettings>> m_camSettings;
    QVector<QPair<int,int>> m_Topology;

    //paint props
    QVector<QColor> m_randomColors;
    bool m_drawJoints = true;
    bool m_drawLines = true;

    QPoint m_camRot = QPoint(0,0);
    float m_zoom = 1.0;

    //mouse tracking
    bool m_leftButton = false;
    bool m_rightButton = false;
    QPoint m_lastMousePos, m_currentMousePos;

    Frame m_actualFrame;

    //some glu mess
    GLUquadricObj *m_sphereQuadric = gluNewQuadric();
    GLUquadricObj *m_cubeQuadric = gluNewQuadric();
public:
    /**
     * @brief OpenGlScene default constructor
     * @param parent
     */
    explicit OpenGlScene(QWidget *parent = 0);

    /**
     * @brief For bottom rectangle, #OpenGlScene needs to know size of #IVirtualRoom.
     * @param m_roomDims Room dimensions
     */
    void setRoomDimensions(QVector3D m_roomDims);

    /**
     * @brief For camera position preview, #OpenGlScene needs to know position
     * and direction vector of every #ICamera in the virtual scene.
     * @param settings All #CameraSettings in #IVirtualRoom
     */
    void setCamerasSettings(QVector<std::shared_ptr<CameraSettings>> settings);

    /**
     * @brief Static function for using #OpenGlScene as singleton.
     * @return Singleton
     */
    static OpenGlScene *getInstance();

public slots:
    /**
     * @brief SLOT which sets actual frame to scene. #OpenGlScene draws every #Line and #Marker in \a frame
     * @param frame
     */
    void setFrame(Frame frame);

private:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void paintScene();
    void paintCameras();
    void paintTopology();
    void paintFrame();

    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void drawLine(const Line &line) const;

    void generateRandomColors(int count);
};

#endif // OPENGLSCENE_H
