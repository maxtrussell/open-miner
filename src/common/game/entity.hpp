#pragma once

#include <glm/glm.hpp>

enum Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    DOWN,
    UP,
};

class Entity {
    public:
        Entity();  // TODO: provide constructor
        void handleMovement(Movement direction, float deltaTime);

        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
        bool alive = false;

    private:
        glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 lookDirection = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        bool flyCam = false;

        float yaw = -90.0f;
        float pitch = 0.0f;
        float speed = 2.5f;
        float sensitivity = 0.1f;
        float fov = 70.0f;

};
