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
        std::cout << "Server: Failed to bind socket on port " << port << std::endl;
    } else {
        std::cout << "Server: Listening on port " << port << std::endl;
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
            case CLIENT_REQUEST_TYPE::DISCONNECT:
                handleDisconnect(req.packet);
                break;
        }
    }
}

void Server::handleIncomingConnection(sf::IpAddress ip, int port) {
    if (connections >= maxConnections) {
        // Reject, server full
        sf::Packet packet;
        packet << SERVER_REQUEST_TYPE::CONNECT_REQUEST_RESULT
               << CONNECTION_RESULT::GAME_FULL;
        socket.send(packet, ip, port);
        std::cout << "Server: Connection reqeust from client rejected, server full ("
                  << ip << ":" << port << ")" << std::endl;
        return;
    }

    // Handle duplicate clients
    for (unsigned int i = 0; i < clients.size(); i++) {
        Client c = clients[i];
        if (c.connected && c.ip == ip && c.port == port) {
            sf::Packet packet;
            packet << SERVER_REQUEST_TYPE::CONNECT_REQUEST_RESULT
                   << CONNECTION_RESULT::DUPLICATE_ID;
            socket.send(packet, ip, port);
            std::cout << "Server: Connection reqeust from client rejected, duplicate id ("
                      << ip << ":" << port << ")" << std::endl;
        }
    }
    
    // Add client to slot
    unsigned int i;
    for (i = 0; i < clients.size(); i++) {
        if (!clients[i].connected)
            break;
    }
    Client client = { i, ip, port, true };
    clients.push_back(client);
    connections++;

    // Broadcast player arrival
    sf::Packet broadcastPack;
    broadcastPack << SERVER_REQUEST_TYPE::PLAYER_JOIN
                  << client.id;
    broadcast(broadcastPack);
    
    // Alert player connection was successful
    sf::Packet packet;
    packet << SERVER_REQUEST_TYPE::CONNECT_REQUEST_RESULT
           << CONNECTION_RESULT::SUCCESS
           << client.id;
    socket.send(packet, ip, port);

    std::cout << "Server: Connection reqeust from client accepted, ID: " << client.id << " ("
        << ip << ":" << port << ")" << std::endl;

}

bool Server::isRunning() {
    return running;
}

void Server::broadcast(sf::Packet packet) {
    for (unsigned int i = 0; i < clients.size(); i++) {
        if (clients[i].connected)
            socket.send(packet, clients[i].ip, clients[i].port);
    }
}

void Server::handleDisconnect(sf::Packet packet) {
    int id;
    packet >> id;

    // Broadcast player disconnect
    sf::Packet broadcastPack;
    broadcastPack << SERVER_REQUEST_TYPE::PLAYER_LEAVE << id;
    broadcast(broadcastPack);

    // Mark player disconnected
    clients[id].connected = false;
    connections--;
}
