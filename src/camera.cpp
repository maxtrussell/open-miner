#include "camera.hpp"

Camera::Camera(
    glm::vec3 pos,
    glm::vec3 dir,
    glm::vec3 worldUp,
    float yaw,
    float pitch,
    float speed,
    float sensitivity,
    float fov
) 
    : pos(pos),
    dir(dir),
    worldUp(worldUp),
    yaw(yaw),
    pitch(pitch),
    speed(speed),
    sensitivity(sensitivity),
    fov(fov)
{
    updateCameraAngles();
}

// inherit Camera constructor directly
FPSCamera::FPSCamera(
    glm::vec3 pos,
    glm::vec3 dir,
    glm::vec3 worldUp,
    float yaw,
    float pitch,
    float speed,
    float sensitivity,
    float fov
) 
    : Camera(pos, dir, worldUp, yaw, pitch, speed, sensitivity, fov)
{}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(pos, pos + dir, worldUp);
}

void Camera::processKeyboard(CAMERA_MOVEMENT direction, float deltaTime) {
    float deltaPos = speed * deltaTime;
    if (direction == CAMERA_MOVEMENT::FORWARD)
        pos += dir * deltaPos;
    if (direction == CAMERA_MOVEMENT::LEFT)
        pos -= glm::normalize(glm::cross(dir, glm::vec3(0.0f, 1.0f, 0.0f))) * deltaPos;
    if (direction == CAMERA_MOVEMENT::BACKWARD)
        pos += -dir * deltaPos;
    if (direction == CAMERA_MOVEMENT::RIGHT)
        pos += glm::normalize(glm::cross(dir, glm::vec3(0.0f, 1.0f, 0.0f))) * deltaPos;
    if (direction == CAMERA_MOVEMENT::UP)
        pos += worldUp * deltaPos;
    if (direction == CAMERA_MOVEMENT::DOWN)
        pos -= worldUp * deltaPos;
}

void FPSCamera::processKeyboard(CAMERA_MOVEMENT direction, float deltaTime) {
    float deltaPos = speed * deltaTime;
    if (direction == CAMERA_MOVEMENT::FORWARD)
        pos += glm::vec3(dir.x, 0.0f, dir.z) * deltaPos;
    if (direction == CAMERA_MOVEMENT::LEFT)
        pos -= glm::normalize(glm::cross(dir, glm::vec3(0.0f, 1.0f, 0.0f))) * deltaPos;
    if (direction == CAMERA_MOVEMENT::BACKWARD)
        pos -= glm::vec3(dir.x, 0.0f, dir.z) * deltaPos;
    if (direction == CAMERA_MOVEMENT::RIGHT)
        pos += glm::normalize(glm::cross(dir, glm::vec3(0.0f, 1.0f, 0.0f))) * deltaPos;
}

void Camera::processMouse(float deltaX, float deltaY) {
    deltaX *= sensitivity;
    deltaY *= sensitivity;

    yaw += deltaX;
    pitch -= deltaY;  // minus to un-invert y

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    // camera stuff
    updateCameraAngles();
}

void Camera::updateCameraAngles() {
    // Calculate new dir
    dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    dir.y = sin(glm::radians(pitch));
    dir.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    dir = glm::normalize(dir);

    // Update right and up
    right = glm::normalize(glm::cross(dir, worldUp));
    up = glm::normalize(glm::cross(right, dir));
    
}
