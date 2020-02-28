
#include <src/client/engine.hpp>

Engine::Engine(sf::IpAddress ip, int port) {
    // Init client
    client = new Client(ip, port);
    client->connect();

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
        // Calculate deltaTime
        float currFrame = glfwGetTime();
        deltaTime = currFrame - lastFrame;

        // Client stuff
        client->update();
        client->handleInput(window);

        // DEBUG stuff
        /*
        Entity ep = *client->entity;
        std::cout << "Position: <" << ep.position.x
                  << ", " << ep.position.y << ", "
                  << ep.position.z << ">" << std::endl;
        */


        // TODO: Draw stuff
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Draw screen
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Wait as necessary for tick
        tick(glfwGetTime() - currFrame);
    } 
    client->disconnect();

    // Cleanly exit
    glfwTerminate();
}

void Engine::framebufferSizeCallback(int width, int height) {
    // TODO
}

void Engine::mouseCallback(double xPos, double yPos) {
    // TODO
}

void Engine::tick(float elapsedTime) {
    int sleepTime = (tickLength - elapsedTime) * 1000;  // milliseconds
    if (sleepTime > 0)
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
}
