#include <ctime>
#include <chrono>
#include <iostream>
#include <thread>

#include "server.hpp"

Server::Server(int port, int maxConnections)
    : port(port), maxConnections(maxConnections)
{
    socket.setBlocking(false);
    // bind the socket to a port
    if (socket.bind(port) != sf::Socket::Done) {
        std::cout << "Failed to bind socket on port " << port << std::endl;
    } else {
        // std::cout << "Listening on port " << port << std::endl;
        running = true;
    }
}

void Server::run(sf::Time timeout) {
    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
        listen();
    }
}

void Server::listen() {
    Request req;
    if (socket.receive(req.packet, req.ip, req.port) == sf::Socket::Done) {
        req.packet >> req.type;
        switch (req.type) {
            case CLIENT_REQUEST_TYPE::CONNECT:
                handleIncomingConnection(req.ip, req.port);
                break;
        }
    }
}

void Server::handleIncomingConnection(sf::IpAddress ip, int port) {
    std::cout << "Connection request received\n";
    if (connections >= maxConnections) {
        // Reject, server full
        sf::Packet packet;
        packet << SERVER_REQUEST_TYPE::CONNECT_REQUEST_RESULT
               << CONNECTION_RESULT::GAME_FULL;
        socket.send(packet, ip, port);
    }

    // TODO: handle duplicate connections
    
    // 1. Add client to slot to avoid race condition
    unsigned int i;
    for (i = 0; i < clients.size(); i++) {
        if (!clients[i].connected)
            break;
    }
    Client client = { i, ip, port, true };
    clients.push_back(client);
    connections++;
    
    sf::Packet packet;
    packet << SERVER_REQUEST_TYPE::CONNECT_REQUEST_RESULT
           << CONNECTION_RESULT::SUCCESS
           << client.id;
    socket.send(packet, ip, port);

    std::cout << "Player connected from " << client.ip << ":"
              << client.port << " with ID: " << client.id << "\n";

    // TODO: let all players know somebody has joined
}

bool Server::isRunning() {
    return running;
}

