#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/UdpSocket.hpp>
#include <SFML/System/Clock.hpp>

#include <src/common/net/requests.hpp>
#include <src/server/entity.hpp>

class Server {
    public:
        int port;

        Server(int port, int maxConnections);
        void run(sf::Time timeout = sf::seconds(7.5));
        void listen();
        bool isRunning();
    private:
        struct Request {
            sf::Packet packet;
            CLIENT_REQUEST_TYPE type;
            sf::IpAddress ip;
            unsigned short port;
        };
        struct Client {
            unsigned int id;
            unsigned int entityId;
            sf::IpAddress ip;
            int port;
            bool connected;
        };

        sf::UdpSocket socket;

        int maxConnections;
        int connections = 0;
        bool running = false;
        std::vector<Client> clients;
        std::vector<Entity> entities;

        void handleDisconnect(sf::Packet packet);
        void handleIncomingConnection(sf::IpAddress, int port);
        void broadcast(sf::Packet packet);
};
