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

#ifndef Room_H
#define Room_H

#include "animation.h"
#include "pointchecker.h"
#include "capturethread.h"

#include <QTime>
#include <QtNetwork/QLocalServer>
#include <QtNetwork/QLocalSocket>


class Edge
{
public:
    Edge(int a, int b, float Eps): m_index1(a), m_index2(b), m_maxError(Eps) {}

    size_t m_index1, m_index2;
    QVector<Line> a ,b;
    float m_maxError;
    QVector <glm::vec3> points;
};

class Room : public QObject
{
    Q_OBJECT

    //basic parameters for project
    QString m_name;
    glm::vec3 m_roomDimensions; //centimeters
    double m_maxError;
    bool m_saved;
    OpenGLWindow *m_openGLWindow = nullptr;

    std::vector <Edge> m_cameraTopology;
    std::vector <CaptureCamera*> m_cameras;

    size_t m_activeCamerasCount;
    size_t m_lastActiveCamIndex;

    bool m_record = false;
    bool m_captureAnimation = false;
    Animation* actualAnimation = nullptr;
    std::vector<Animation*> animations;

    //pipe
    bool m_usePipe = false;
    QLocalServer *server = nullptr;
    QLocalSocket *socket = nullptr;

    //parallel
    QWaitCondition allLines;

    //cams
    std::vector <bool> haveResults;
    QVector<QVector<Line>> results;

    std::vector <worker*> workers;
    std::vector <QThread*> workerthreads;

    //intersections
    QTime timer;

    std::vector<glm::vec3> points;
    std::vector<glm::vec2> points2D; //2Drecording

    PointChecker checker;
    std::vector<Point> labeledPoints;

public:
    Room(OpenGLWindow *m_openGLWindow = nullptr, glm::vec3 dimensions = glm::vec3(0.0f,0.0f, 0.0f), float eps = 0.5, QString m_name = "Default Project");
    //Room(std::string file);
    ~Room();

    void fromVariantMap(OpenGLWindow *opengl ,QVariantMap &varMap);
    QVariantMap toVariantMap();

    void AddCamera(CaptureCamera *cam);
    void RemoveCamera(size_t index);
    //void Save(std::ofstream &file);

    void MakeTopology();
    void resolveTopologyDuplicates();

    void TurnOnCamera(size_t index);
    void TurnOffCamera(size_t index);
    void ShowCameraVideo(size_t index){m_cameras[index]->Show();}
    void HideCameraVideo(size_t index){m_cameras[index]->Hide();}

    void CaptureAnimationStart();
    void setPipe(bool pipe);
    Animation *CaptureAnimationStop();
    void RecordingStart();
    void RecordingStop();

    void setDimensions(glm::vec3 dims);
    void setName(QString name){this->m_name = name;}
    void setEpsilon(float size);
    void setNumberOfPoints(size_t nOfPts);

    QString getName() const {return m_name;}
    glm::vec3 getDimensions() const {return m_roomDimensions;}
    int getWidth()const {return m_roomDimensions.x;}
    int getLength()const {return m_roomDimensions.y;}
    float getEpsilon() const {return m_maxError;}
    bool getSaved() const {return m_saved;}
    QVector<QVector<Line>> getLines() const {return results;}
    std::vector <CaptureCamera*> getcameras()const {return m_cameras;}

    void setOpenglWindow(OpenGLWindow * opengl) {this->m_openGLWindow = opengl;}

    static void Intersection(Edge &camsEdge);

signals:
    void startWork();
    void stopWork();
    void startWork2D();

private slots:
    void ResultReady(QVector<Line> lines);
    void record2D();
    void handleConnection();
    void handleDisconnected();

private:
    void sendMessage(std::vector<Pnt> &Points);
    void sendMessageString(std::string str);

    void Intersections();

    void weldPoints(std::vector<glm::vec3> &points);

};

#endif // Room_H
