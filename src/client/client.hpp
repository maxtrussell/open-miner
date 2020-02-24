#pragma once

#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/UdpSocket.hpp>

#include <src/common/net/requests.hpp>

class Client {
    public:
        Client(sf::IpAddress ip, int port);
        bool connect();
        void update();

    private:
        struct Request {
            sf::Packet packet;
            SERVER_REQUEST_TYPE type;
        };

        sf::IpAddress serverIp;
        sf::UdpSocket socket;

        int serverPort;
        int id;

        bool connected = false;

        bool listen(Request &response);
        void handlePlayerJoin(sf::Packet packet);
        void handlePlayerLeave(sf::Packet packet);
};
