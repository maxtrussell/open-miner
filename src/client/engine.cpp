
#include <src/client/engine.hpp>

Engine::Engine(sf::IpAddress ip, int port) {
    // Init client
    Client client(ip, port);
    client.connect();

    // Init glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // GLFW: window creation
    window = glfwCreateWindow(
        screenWidth,
        screenHeight,
        "Open Miner -- Poverty Minecraft",
        NULL,
        NULL
    );
    if (window == NULL) {
        std::cout << "Client: Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    // GLAD: Load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Client: Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return;
    }
}

// Main client loop
void Engine::run() {
    while (!glfwWindowShouldClose(window)) {
        processInput();

        // TODO: Draw stuff
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Draw scree
        glfwSwapBuffers(window);
        glfwPollEvents();
    } 

    // Cleanly exit
    glfwTerminate();
}

void Engine::processInput() {
    // TODO
}

void Engine::framebufferSizeCallback(int width, int height) {
    // TODO
}

void Engine::mouseCallback(double xPos, double yPos) {
    // TODO
}
