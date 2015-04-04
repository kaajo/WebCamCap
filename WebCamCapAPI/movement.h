#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <glm/glm.hpp>

/**
 * @author Miroslav Krajicek <409917@mail.muni.cz>
 * @class Movement movement.h
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
     * @brief direction getter
     * @return returns Direction of Movement. Default is glm::vec3(0.0f,0.0f,0.0f)
     */
    glm::vec3 direction() const;

    /**
     * @brief direction setter
     * @param direction of Movement
     */
    void setDirection(const glm::vec3 &direction);

    /**
     * @brief size getter
     * @return size of Movement
     */
    float size() const;

    /**
     * @brief size setter
     * @param size of Movement
     */
    void setSize(float size);
};

#endif // MOVEMENT_H
