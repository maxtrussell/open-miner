#include <iostream>

#include "entity.hpp"

Entity::Entity() {}

void Entity::handleMovement(Movement direction, float deltaTime) {
    float deltaPos = speed * deltaTime;
    if (direction == Movement::FORWARD)
        position += glm::vec3(lookDirection.x, 0.0f, lookDirection.z) * deltaPos;
    if (direction == Movement::LEFT)
        position -= glm::normalize(glm::cross(lookDirection, glm::vec3(0.0f, 1.0f, 0.0f))) * deltaPos;
    if (direction == Movement::BACKWARD)
        position -= glm::vec3(lookDirection.x, 0.0f, lookDirection.z) * deltaPos;
    if (direction == Movement::RIGHT)
        position += glm::normalize(glm::cross(lookDirection, glm::vec3(0.0f, 1.0f, 0.0f))) * deltaPos;
}
