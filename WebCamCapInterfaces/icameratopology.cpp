#include "icameratopology.h"

double ICameraTopology::m_maxError;

QVector3D ICameraTopology::getRoomDimensions() const
{
    return m_roomDimensions;
}

void ICameraTopology::setRoomDimensions(const QVector3D &roomDimensions)
{
    m_roomDimensions = roomDimensions;
}

double ICameraTopology::getMaxError()
{
    return m_maxError;
}

void ICameraTopology::setMaxError(double maxError)
{
    m_maxError = maxError;
}
ICameraTopology::ICameraTopology(QVector3D roomDims, double maxError, QObject *parent) :
    QObject(parent)
{
    m_maxError = maxError;
    m_roomDimensions = roomDims;
}

ICameraTopology::~ICameraTopology()
{
    
}

ICamera *ICameraTopology::getCamera(QString name)
{
    return *(std::find_if(m_cameras.begin(), m_cameras.end(), [=](ICamera* cam){return cam->settings()->name() == name;}));
}


