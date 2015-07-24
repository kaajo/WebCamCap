#include "polygoncameratopology.h"

#include <QPair>
#include <QDebug>
#include <QtConcurrent>

QMap<ICamera*, QVector<Line>> PolygonCameraTopology::m_resultLines;

PolygonCameraTopology::PolygonCameraTopology(QVector3D roomDims, double maxError, QObject *parent) : ICameraTopology(roomDims, maxError ,parent)
{
    m_waitCondition = new QWaitCondition;
}

PolygonCameraTopology::~PolygonCameraTopology()
{

}

void PolygonCameraTopology::addCameras(QVector<ICamera*> cameras)
{
    foreach (ICamera* cam, cameras) {

        if(m_cameras.contains(cam))
        {
            continue;
        }

        cam->setWaitCondition(m_waitCondition);

        if(cam->settings()->turnedOn())
        {
            ++m_turnedOnCamerasCounter;
        }

        m_cameras.push_back(cam);

        QThread *thread = new QThread();
        cam->moveToThread(thread);
        m_cameraThreads.push_back(thread);

        ///connect( this, SIGNAL(startWork2D()), this, SLOT(record2D()));
        connect(this, &PolygonCameraTopology::startRecording, cam, &ICamera::startWork);
        connect(this, &PolygonCameraTopology::stopRecording, cam, &ICamera::stopWork);
        connect(cam, &ICamera::results,this,&PolygonCameraTopology::handleCameraResults, Qt::QueuedConnection);
        connect(cam, &ICamera::finished, thread, &QThread::quit);
        connect(cam, &ICamera::finished, cam, &ICamera::deleteLater);
        connect(thread, &QThread::finished, thread, &QThread::deleteLater);

        thread->start();
        /*
         *
    workers.push_back(new worker(&m_linesWaitCondition, cam));

         * */
    }

    resolveEdges();
}

void PolygonCameraTopology::removeCamera(ICamera *camera)
{

}

void PolygonCameraTopology::record(bool start)
{
    if(start)
    {
        emit startRecording();
        m_frameTimer.start();
    }
    else
    {
        emit stopRecording();
    }
}

void PolygonCameraTopology::resolveEdges()
{
    m_topology.clear();
/*
    QVector3D pos1, pos2;
    QVector3D dir1, dir2;

    float min = 181.0f, temp_angle;
    int min_index;

    //get
    for(int i = 0; i < m_cameras.size(); i++)
    {
        min_index = -1;

        pos1 = m_cameras[i]->settings()->globalPosition();
        dir1 = m_cameras[i]->settings()->getDirectionVector().toVector3D();

        for(int j = i+1; j < m_cameras.size(); j++)
        {

            pos2 = m_cameras[j]->settings()->globalPosition();
            dir2 = m_cameras[j]->settings()->getDirectionVector().toVector3D();

            temp_angle = Line::lineAngle(QVector2D(dir1.x(), dir1.y()),QVector2D(dir2.x(), dir2.y()));

            if(qAbs(temp_angle) < min)
            {
                min = temp_angle;
                min_index = j;
            }
        }

        if(min_index != -1 &&  min_index != i)
        {
            TopologyEdge edge(m_cameras[i],m_cameras[min_index]);

            m_topology.push_back(edge);
        }
    }

    for(int i = 0; i < m_topology.size(); i++)
    {
        ICamera* index1 = m_topology[i].m_camera1;
        ICamera* index2 = m_topology[i].m_camera2;

        for(int j = 0; j < m_topology.size(); j++)
        {
            if(m_topology[j].m_camera2 == index1 && m_topology[j].m_camera1 == index2)
            {
                m_topology.erase(m_topology.begin()+j);
            }
        }
    }

    std::cout << "new camtopology size:" << m_topology.size() << std::endl;
    */

    if(m_cameras.size() < 2)
    {
        qDebug() << "only 1 camera -> no topology";
        return;
    }

    QVector3D dir1, dir2;

    QVector<QPair<double, TopologyEdge>> allEdges;

    //get
    for(int i = 0; i < m_cameras.size(); i++)
    {
        dir1 = m_cameras[i]->settings()->getDirectionVector().toVector3D();

        for(int j = i+1; j < m_cameras.size(); j++)
        {
            dir2 = m_cameras[j]->settings()->getDirectionVector().toVector3D();

            allEdges.push_back(QPair<double,TopologyEdge>(Line::lineAngle(dir1, dir2), {m_cameras[i], m_cameras[j]}));
        }
    }

    QVector<ICamera*> usedCams;
    QVector<TopologyEdge> topology;

    std::sort(allEdges.begin(), allEdges.end(), [](const QPair<double, TopologyEdge> &pair1 , const QPair<double, TopologyEdge> &pair2){return pair1.first < pair2.first;});

    auto it = allEdges.begin();

    while(topology.size() != m_cameras.size())
    {
        if(it == allEdges.end())
        {
            break;
        }

        if(! usedCams.contains(it->second.m_camera1))
        {
            usedCams.push_back(it->second.m_camera1);
        }

        if(! usedCams.contains(it->second.m_camera2))
        {
            usedCams.push_back(it->second.m_camera2);
        }

        topology.push_back(it->second);

        ++it;
    }

    m_topology = topology;

    qDebug() << "new camtopology size: " << m_topology.size();

    for(int i = 0; i < topology.size(); i++)
    {
        qDebug() << i << " " << topology[i].m_camera1->settings()->globalPosition()
                             << topology[i].m_camera2->settings()->globalPosition();
    }
}

void PolygonCameraTopology::intersections()
{
    QVector<QVector<QVector3D>> points = QtConcurrent::blockingMapped<QVector<QVector<QVector3D>>>(m_topology, PolygonCameraTopology::intersection);

    QVector<QVector3D> pointsFlatten;

    for(auto pts: points)
    {
        pointsFlatten+=pts;
    }

    auto labeledPoints = m_pointChecker.solvePointIDs(pointsFlatten);

    normaliseCoords(labeledPoints, m_roomDimensions);

    emit frameReady(Frame(m_frameTimer.elapsed(), labeledPoints, m_resultLines.values().toVector()));

    QCoreApplication::processEvents();
}

void PolygonCameraTopology::normaliseCoords(QVector<Marker> &points, QVector3D roomSize)
{
    for(Marker &pnt: points)
    {
        auto position = pnt.position();

        pnt.setPosition({position.x() / roomSize.x() , position.y() / roomSize.y(), position.z() / roomSize.z()});
    }
}

QVector<QVector3D> PolygonCameraTopology::intersection(TopologyEdge edge)
{
    QVector<QVector3D> retVal;

    for(int i = 0; i < m_resultLines[edge.m_camera1].size(); i++)
    {
        for(int j = 0; j < m_resultLines[edge.m_camera2].size(); j++)
        {
            QVector3D tempPoint;
            if(Line::intersection(m_resultLines[edge.m_camera1][i], m_resultLines[edge.m_camera2][j], m_maxError, tempPoint))
            {
                retVal.push_back(tempPoint);
            }
        }
    }

    return retVal;
}

void PolygonCameraTopology::handleCameraSettingsChange(CameraSettings::CameraSettingsType type)
{
    CameraSettings *settings = qobject_cast<CameraSettings*>(sender());

    switch (type) {
    case CameraSettings::CameraSettingsType::TURNEDON:
        if(settings->turnedOn())
        {
            ++m_turnedOnCamerasCounter;
        }
        else
        {
            --m_turnedOnCamerasCounter;
        }
        break;
    default:
        break;
    }
}

void PolygonCameraTopology::handleCameraResults(QVector<Line> lines)
{
    cv::waitKey(1);

    ++m_resultsCounter;

    m_resultLines[qobject_cast<ICamera*>(sender())] = lines;

    if(m_resultsCounter == m_cameras.size())
    {
        intersections();

        qDebug() << m_frameTimer.elapsed();

        m_frameTimer.restart();
        m_waitCondition->wakeAll();

        m_resultsCounter = 0;
    }

    QCoreApplication::processEvents();
}


//#######################################################################

#include "markercamera.h"

const QString camerasKey("cameras");

QVariantMap PolygonCameraTopology::toVariantMap()
{
    QVariantMap retVal;

    QVariantList camerasList;

    foreach (ICamera *camera, m_cameras) {
        camerasList << camera->toVariantMap();
    }

    retVal[camerasKey] = camerasList;

    return retVal;
}

bool PolygonCameraTopology::fromVariantMap(QVariantMap varMap)
{
    QVariantList camerasList = varMap[camerasKey].toList();

    QVector<ICamera*> cameras;

    foreach (const QVariant variant, camerasList) {
        ICamera *camera = new MarkerCamera(variant.toMap(), m_waitCondition);

        cameras.push_back(camera);
    }

    addCameras(cameras);

    return true;
}
