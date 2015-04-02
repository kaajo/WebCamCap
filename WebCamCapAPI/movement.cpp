#include "movement.h"


glm::vec3 Movement::direction() const
{
    return m_direction;
}

void Movement::setDirection(const glm::vec3 &direction)
{
    m_direction = direction;
}

float Movement::size() const
{
    return m_size;
}

void Movement::setSize(float size)
{
    m_size = size;
}
Movement::Movement(glm::vec3 direction, float size) :m_direction(direction), m_size(size)
{
}
