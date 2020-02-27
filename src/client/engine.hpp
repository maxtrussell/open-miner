#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SFML/Network/IpAddress.hpp>

#include <src/camera.hpp>
#include <src/shader.hpp>
#include <src/client/client.hpp>


class Engine {
    public:
        Engine(sf::IpAddress ip, int port);
        void run();  // to be the main loop

    private:
        int screenWidth = 800;
        int screenHeight = 600;
        float deltaTime = 0.0f;
        float lastFrame = 0.0f;
        bool hasFocus = false;
        float lastX = screenWidth / 2.0f;
        float lastY = screenHeight / 2.0f;

        Camera camera;
        GLFWwindow* window;
        Client* client;

        void framebufferSizeCallback(int width, int height);
        void mouseCallback(double xPos, double yPos);
        void processInput();
};
