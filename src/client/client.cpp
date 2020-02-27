#include <iostream>

#include <glm/glm.hpp>

#include "client.hpp"

Client::Client(sf::IpAddress serverIp, int serverPort)
    : serverIp(serverIp), serverPort(serverPort) {}

bool Client::connect() {
    // Make request packet
    sf::Packet packet;
    packet << ClientRequest::CONNECT;

    // Bind a port for listening
    if (socket.bind(sf::Socket::AnyPort) != sf::Socket::Done) {
        std::cout << "Client: Failed to bind socket on port "
                  << socket.getLocalPort() << std::endl;
    } else {
        std::cout << "Client: Listening on port " << socket.getLocalPort() << std::endl;
    }

    // Send request
    if (socket.send(packet, serverIp, serverPort) == sf::Socket::Done) {
        Request response;
        if (listen(response)) {
            ConnectionResult result;
            response.packet >> result;
            switch(result) {
                case ConnectionResult::SUCCESS:
                    response.packet >> id;
                    connected = true;
                    std::cout << "Client: Connected, ID: " << id << std::endl;
                    socket.setBlocking(false);
                    return true;
                case ConnectionResult::GAME_FULL:
                    std::cout << "Client: Could not connect. Server is full." << std::endl;
                    break;
                case ConnectionResult::DUPLICATE_ID:
                    std::cout << "Client: Unable to join on same connection and port" << std::endl;
                    break;
            }
        } else {
            std::cout << "Client: Connection failed." << std::endl;
            return false;
        }
    }
    return false;
}

void Client::update() {
    Request req; 
    if (listen(req)) {
        switch(req.type) {
            case ServerRequest::PLAYER_JOIN:
                handlePlayerJoin(req.packet);
                break;
            case ServerRequest::PLAYER_LEAVE:
                handlePlayerLeave(req.packet);
                break;
            case ServerRequest::ENTITY_UPDATE:
                handleEntityUpdate(req.packet);
                break;
        }
    }
}

void Client::disconnect() {
    sf::Packet packet;
    packet << ClientRequest::DISCONNECT << id;
    socket.send(packet, serverIp, serverPort);
}

bool Client::listen(Request &response) {
    sf::IpAddress ip;
    unsigned short port;
    if (socket.receive(response.packet, ip, port) == sf::Socket::Done) {
       response.packet >> response.type;
       return true; 
    }
    return false;
}

void Client::handlePlayerJoin(sf::Packet packet) {
    int playerId;
    packet >> playerId;
    std::cout << "Client: Player has joined: " << playerId << std::endl;
}

void Client::handlePlayerLeave(sf::Packet packet) {
    int playerId;
    packet >> playerId;
    std::cout << "Client: Player has disconnected: " << playerId << std::endl;
}

void Client::handleEntityUpdate(sf::Packet packet) {
    int numEntities;
    packet >> numEntities;
    for (int i = 0; i < numEntities; i++) {
        int entityId;
        float x, y, z;
        packet >> entityId >> x >> y >> z;
        if (entityId == id)  // if self
            continue;
        Entity* ep = &entities[entityId];
        ep->position = glm::vec3(x, y, z);
        ep->alive = true;
    }
}
