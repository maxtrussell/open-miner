#include <iostream>
#include <thread>
#include <string.h>

#include <SFML/Network/IpAddress.hpp>

#include "client/client.hpp"
#include "server/server.hpp"

// TODO: config
int PORT = 5003;
int MAX_CONNECTIONS = 10;

enum Mode {
    SERVER,
    CLIENT,
    INVALID,
};

int launchServer();
Mode parseArgs(int argc, char* argv[]);


int main(int argc, char *argv[]) {
    Mode mode = parseArgs(argc, argv);
    if (mode == Mode::INVALID) {
        std::cout << "Invalid mode" << std::endl;
        return -1;
    }


    // Launch server
    if (mode == Mode::SERVER) {
        std::cout << "Starting in server mode" << std::endl;
        std::thread serverThread(launchServer);

        // Wait for server
        serverThread.join();
        std::cout << "Exiting" << std::endl;
    }

    // Launch client
    if (mode == Mode::CLIENT) {
        std::cout << "Starting in client mode" << std::endl;
        sf::IpAddress serverIp("127.0.0.1");
        Client client(serverIp, PORT);
        client.connect();

        // TODO
        while (true)
            ;
    }

    return 0;
}

// Wrapper for Server for easy threading
int launchServer() {
    Server server(PORT, MAX_CONNECTIONS);
    server.run();
    return 0;
}

Mode parseArgs(int argc, char* argv[]) {
    bool server = false, client = false;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--server") == 0) {
            server = true;
        } else if (strcmp(argv[i], "--client") == 0) {
            client = true;
        }
        if (server && client)
            return Mode::INVALID;
    }
    if (!server && !client)
        return Mode::INVALID;
    return server ? Mode::SERVER : Mode::CLIENT;
}
