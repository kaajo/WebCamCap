#include "marker.h"


QVector3D Marker::position() const
{
    return m_position;
}

void Marker::setPosition(const QVector3D &position)
{
    m_position = position;
}

int Marker::id() const
{
    return m_id;
}

void Marker::setId(int id)
{
    m_id = id;
}

Marker Marker::operator -(Marker &point)
{
    return {m_id, m_position - point.position()};
}

Marker Marker::operator *(float &val)
{
    return {m_id, m_position * val};
}

Marker Marker::operator +(Marker point)
{
    return {m_id, m_position + point.position()};
}

Marker::Marker()
{

}

Marker::Marker(int id, QVector3D positionVector)
{
    m_id = id;
    m_position = positionVector;
}

Marker::Marker(int id, glm::vec3 positionVector)
{
    m_id = id;
    m_position = QVector3D(positionVector.x, positionVector.y, positionVector.z);
}
