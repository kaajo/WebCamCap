#ifndef POINT_H
#define POINT_H

#include <glm/glm.hpp>

/**
 * @author Miroslav Krajicek <409917@mail.muni.cz>
 * @class Point point.h
 * @brief Represents Point in 3D space with unique ID.
 * This class can be used also for 2D points, z axis has then zero value.
 * @date 2015
 */

class Point
{
    size_t m_id = -1;
    glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 0.0f);
public:

    /**
     * @brief Point constructs Point with default values (Used for void QVector::reserve(int size) )
     */
    Point();

    /**
     * @brief Point constructor
     * @param id Point unique ID
     * @param position position in 3D space.
     */
    Point(size_t id, glm::vec3 position);

    Point operator -(Point &point);
    Point operator *(float &val);
    Point operator +(Point point);

    /**
     * @brief position getter
     * @return Position of Point. Default position is glm::vec3(0.0f,0.0f,0.0f)
     */
    glm::vec3 position() const;

    void setPosition(const glm::vec3 &position);
    void setPositionX(const float &val);
    void setPositionY(const float &val);
    void setPositionZ(const float &val);

    /**
     * @brief ID getter
     * @return ID of Point. Default id is -1.
     */
    size_t id() const;

    void setId(const size_t &id);
};

#endif // POINT_H
