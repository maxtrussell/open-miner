#pragma once

#include <glm/glm.hpp>

// TODO: convert to class when adding mobs
struct Entity {
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 lookDirection = glm::vec3(0.0f, 0.0f, -1.0f);
    bool alive = false;
};
