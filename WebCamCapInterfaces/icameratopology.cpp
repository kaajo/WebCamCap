#include "icameratopology.h"

ICameraTopology::ICameraTopology(QObject *parent) :
    QObject(parent)
{
}

ICameraTopology::~ICameraTopology()
{

}

ICamera *ICameraTopology::getCamera(QString name)
{
    return *(std::find_if(m_cameras.begin(), m_cameras.end(), [=](ICamera* cam){return cam->settings()->name() == name;}));
}


