#include "polygoncameratopology.h"

#include <QtConcurrent>

PolygonCameraTopology::PolygonCameraTopology(QObject *parent) : ICameraTopology(parent)
{

}

QVariantMap PolygonCameraTopology::toVariantMap()
{
    QVariantMap retVal;

    return retVal;
}

void PolygonCameraTopology::fromVariantMap(QVariantMap varMap)
{

}

void PolygonCameraTopology::addCameras(QVector<ICamera*> cameras)
{
    m_cameras.append(cameras);

    resolveEdges();
}

void PolygonCameraTopology::removeCamera(ICamera *camera)
{

}

void PolygonCameraTopology::record(bool start)
{

}

void PolygonCameraTopology::resolveEdges()
{

}

void PolygonCameraTopology::nextFrame()
{
}

