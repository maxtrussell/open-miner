#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum CAMERA_MOVEMENT {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    DOWN,
    UP,
};

// Fly camera by default
class Camera {
public:
    // Camera attributes
    glm::vec3 pos;
    glm::vec3 dir;
    glm::vec3 right;
    glm::vec3 up;
    glm::vec3 positiveX;
    glm::vec3 positiveY;
    glm::vec3 positiveZ;
    glm::vec3 worldUp;

    // Euler angles
    float yaw;
    float pitch;

    // Camera options
    float speed;
    float sensitivity;
    float fov;

    Camera(
        glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 dir = glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = -90.0f,
        float pitch = 0.0f,
        float speed = 2.5f,
        float sensitivity = 0.1f,
        float fov = 70.0f
    );

    glm::mat4 getViewMatrix();
    void processKeyboard(CAMERA_MOVEMENT direction, float deltaTime);
    void processMouse(float deltaX, float deltaY);

private:
    void updateCameraAngles();
};

class FPSCamera : public Camera {
public:
    FPSCamera(
        glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 dir = glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = -90.0f,
        float pitch = 0.0f,
        float speed = 2.5f,
        float sensitivity = 0.1f,
        float fov = 45.0f
    );

    void processKeyboard(CAMERA_MOVEMENT direction, float deltaTime);
};
