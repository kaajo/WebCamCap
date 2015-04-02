#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <glm/glm.hpp>

/**
 * @author Miroslav Krajicek <409917@mail.muni.cz>
 * @class Movement
 * @brief Small class used to keep information about direction and size of movement.
 * @date 2015
 */

class Movement
{
    glm::vec3 m_direction = glm::vec3(0.0f, 0.0f, 0.0f);
    float m_size = 0.0f;
public:

    /**
     * @brief Movement constructor
     * @param direction vector of movement.
     * @param size of movement in % of screen(2D) or room (3D).
     */

    Movement(glm::vec3 direction, float size);

    /**
     * @brief direction
     * @return
     */
    glm::vec3 direction() const;

    /**
     * @brief setDirection
     * @param direction
     */
    void setDirection(const glm::vec3 &direction);

    /**
     * @brief size
     * @return
     */
    float size() const;

    /**
     * @brief setSize
     * @param size
     */
    void setSize(float size);
};

#endif // MOVEMENT_H
