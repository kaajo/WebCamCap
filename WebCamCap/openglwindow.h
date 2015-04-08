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

#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <QCloseEvent>
#include <QtOpenGL/QGLWidget>

#include "line.h"
#include "pointchecker.h"

class OpenGLWindow : public QGLWidget
{
    Q_OBJECT

    //paint props
    std::vector<std::vector<int>> randomColors;
    bool mdrawJoints, mdrawLines, mdrawBones, twoDimensions;
    glm::vec3 camRot;
    float zoom;

    //Room props
    glm::vec3 roomDims;
    QImage texture;
    unsigned int texId;

    //Structure
    QVector<QVector<Line> > lines;
    std::vector<Point> joints;

    //mouse tracking
    bool leftButton;
    glm::vec2 lastMousePos, currentMousePos;

public:
    explicit OpenGLWindow(QWidget *parent = 0);

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void setRoomDims(glm::vec3 dims);
    void setDrawJoints(bool draw){mdrawJoints = draw;}
    void setDrawLines(bool draw){mdrawLines = draw;}
    void setFrame(std::vector<Point> pts, QVector<QVector<Line>> lns = QVector<QVector<Line> >());

signals:

public slots:

private:   
    //mouse events
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void limitsCamRot();
    void countView();

    void drawLine(const Line &l);

    void drawLines();
    void drawJoints();
    void drawFloor();

    void DefaultView();

    void loadTexture();

    void generateNewRandomColor();
};

#endif // OPENGLWINDOW_H
