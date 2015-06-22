#include "marker.h"


QVector3D Marker::direction() const
{
    return m_direction;
}

void Marker::setDirection(const QVector3D &direction)
{
    m_direction = direction;
}

int Marker::id() const
{
    return m_id;
}

void Marker::setId(int id)
{
    m_id = id;
}

Marker::Marker()
{

}

Marker::Marker(int id, QVector3D directionVector)
{
    m_id = id;
    m_direction = directionVector;
}
