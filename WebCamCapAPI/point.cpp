    #include "point.h"


glm::vec3 Point::position() const
{
    return m_position;
}

void Point::setPosition(const glm::vec3 &position)
{
    m_position = position;
}

void Point::setPositionX(const float &val)
{
    m_position = glm::vec3(val, m_position.y, m_position.z);
}

void Point::setPositionY(const float &val)
{
    m_position = glm::vec3(m_position.x, val, m_position.z);
}

void Point::setPositionZ(const float &val)
{
    m_position = glm::vec3(m_position.x, m_position.y, val);
}

size_t Point::id() const
{
    return m_id;
}

void Point::setId(const size_t &id)
{
    m_id = id;
}

Point::Point()
{
}

Point::Point(size_t id, glm::vec3 position) : m_id(id), m_position(position)
{
}

Point Point::operator -(Point &point)
{
    return {m_id, m_position - point.position()};
}

Point Point::operator *(float &val)
{
    return {m_id, m_position * val};
}

Point Point::operator +(Point point)
{
    return {m_id, m_position + point.position()};
}
