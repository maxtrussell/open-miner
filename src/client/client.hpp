#pragma once

#include <array>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/UdpSocket.hpp>

#include <src/common/net/requests.hpp>
#include <src/common/game/entity.hpp>

class Client {
    public:
        Client(sf::IpAddress ip = sf::IpAddress::LocalHost, int port = 5003);
        bool connect();
        void disconnect();
        void update();
        void handleInput(GLFWwindow* window);

        Entity* entity = new Entity();

    private:
        struct Request {
            sf::Packet packet;
            ServerRequest type;
        };

        sf::IpAddress serverIp;
        sf::UdpSocket socket;

        // TODO: remove hardcoding
        std::array<Entity, 512> entities;

        int serverPort;
        int id;

        bool connected = false;

        bool listen(Request &response);
        void handlePlayerJoin(sf::Packet packet);
        void handlePlayerLeave(sf::Packet packet);
        void handleEntityUpdate(sf::Packet packet);
        void sendInput(Movement m);
};
