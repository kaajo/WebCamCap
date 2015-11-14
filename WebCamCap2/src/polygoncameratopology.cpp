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

        connect(this, &PolygonCameraTopology::startRecording, cam, &ICamera::startWork);
        connect(this, &PolygonCameraTopology::stopRecording, cam, &ICamera::stopWork);
        connect(this, &PolygonCameraTopology::startRecording2D, this, &PolygonCameraTopology::record2D);
        connect(cam, &ICamera::results,this,&PolygonCameraTopology::handleCameraResults, Qt::QueuedConnection);
        connect(cam, &ICamera::finished, thread, &QThread::quit);
        connect(cam, &ICamera::finished, cam, &ICamera::deleteLater);
        connect(thread, &QThread::finished, thread, &QThread::deleteLater);

        thread->start();
    }

    resolveTopologyEdges();
}

void PolygonCameraTopology::removeCamera(ICamera *camera)
{
    ///TODO
}

void PolygonCameraTopology::record(bool start)
{
    int turnedOnCams = 0;

    for(ICamera *camera :m_cameras)
    {
        if(camera->settings()->turnedOn())
        {
            ++turnedOnCams;
        }
    }

    m_turnedOnCamerasCounter = turnedOnCams;

    if(start == m_record)
    {
        return;
    }

    if(start)
    {
        m_record = true;

        //if there is only 1 camera turned on start 2D
        if(m_turnedOnCamerasCounter <= 1)
        {
            qDebug() << "only one camera";
            emit startRecording2D();
        }
        else
        {
            emit startRecording();
            m_frameTimer.start();
        }
    }
    else
    {
        m_record = false;

        emit stopRecording();
    }
}

void PolygonCameraTopology::resolveTopologyEdges()
{
    m_topology.clear();

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

    while(usedCams.size() <= m_cameras.size())
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

    auto labeledPoints = m_pointChecker.solvePointIDs(mergePoints(m_pointChecker.getNumOfPoints(), points));

    normaliseCoords(labeledPoints, m_roomDimensions);

    emit frameReady(Frame(m_frameTimer.elapsed(), labeledPoints, m_resultLines.values().toVector()));

    QCoreApplication::processEvents();
}

QVector<QVector3D> PolygonCameraTopology::mergePoints(size_t targetNumberOfPoints, const QVector<QVector<QVector3D>> &points)
{
    QVector<QVector3D> retVal;

    for(auto pts: points)
    {
        retVal+=pts;
    }

    return retVal;
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

void PolygonCameraTopology::setNumberOfPoints(int numberOfPoints)
{
    m_pointChecker.setNumOfPoints(numberOfPoints);
}

void PolygonCameraTopology::record2D()
{
    while(m_record)
    {
        auto it = std::find_if(m_cameras.begin(), m_cameras.end(), [](ICamera *camera){return camera->settings()->turnedOn();});

        auto labeledPoints= m_pointChecker.solvePointIDs((*it)->nextFrame2D());

        normaliseCoords(labeledPoints, QVector3D((*it)->settings()->resolution(), 1.0));

        emit frameReady(Frame(m_frameTimer.elapsed(),labeledPoints, {}));

        m_frameTimer.restart();

        QCoreApplication::processEvents();
    }
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
        qDebug() << "# cams turned on" <<m_turnedOnCamerasCounter;
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
