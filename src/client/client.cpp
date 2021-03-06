#include <iostream>

#include "client.hpp"

Client::Client(sf::IpAddress ip, int port)
    : serverIp(ip), serverPort(port)
{}

bool Client::connect() {
    // Make request packet
    sf::Packet packet;
    packet << ClientRequest::CONNECT;

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
                    std::cout << "Connected, ID: " << id << std::endl;
                    socket.setBlocking(false);
                    return true;
                case ConnectionResult::GAME_FULL:
                    std::cout << "Could not connect. Server is full." << std::endl;
                    break;
                case ConnectionResult::DUPLICATE_ID:
                    std::cout << "Unable to join on same connection and port" << std::endl;
                    break;
            }
        } else {
            std::cout << "Connection failed." << std::endl;
            return false;
        }
    }
    return false;
}

void Client::update() {
    Request req; 
    while (listen(req)) {
        switch(req.type) {
            case ServerRequest::PLAYER_JOIN:
                handlePlayerJoin(req.packet);
                break;
            case ServerRequest::PLAYER_LEAVE:
                handlePlayerLeave(req.packet);
                break;
        }
    }
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
    int id;
    packet >> id;
    std::cout << "Client: Player has joined: " << id << std::endl;
}

void Client::handlePlayerLeave(sf::Packet packet) {
    int id;
    packet >> id;
    std::cout << "Client: Player has disconnected: " << id << std::endl;
}
