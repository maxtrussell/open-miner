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

    // TODO: Populate, hack
    for (int i = 0; i < maxConnections; i++) {
        Entity ent;
        entities.push_back(ent);

        Client c;
        clients.push_back(c);
    }
}

void Server::run(sf::Time timeout) {
    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
        listen();
        // update(); TODO
        broadcastWorld();
    }
}

void Server::listen() {
    Request req;
    while (socket.receive(req.packet, req.ip, req.port) == sf::Socket::Done) {
        req.packet >> req.type;
        switch (req.type) {
            case ClientRequest::CONNECT:
                handleIncomingConnection(req.ip, req.port);
                break;
            case ClientRequest::DISCONNECT:
                handleDisconnect(req.packet);
                break;
            case ClientRequest::UPDATE_POSITION:
                handleUpdatePosition(req.packet);
                break;
        }
    }
}

void Server::handleIncomingConnection(sf::IpAddress ip, int port) {
    if (connections >= maxConnections) {
        // Reject, server full
        sf::Packet packet;
        packet << ServerRequest::CONNECT_REQUEST_RESULT
               << ConnectionResult::GAME_FULL;
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
            packet << ServerRequest::CONNECT_REQUEST_RESULT
                   << ConnectionResult::DUPLICATE_ID;
            socket.send(packet, ip, port);
            std::cout << "Server: Connection reqeust from client rejected, duplicate id ("
                      << ip << ":" << port << ")" << std::endl;
        }
    }
    
    // Get client id
    unsigned int i;
    for (i = 0; i < clients.size(); i++) {
        if (!clients[i].connected)
            break;
    }

    // Get client entityId
    Entity clientEnt;
    clientEnt.alive = true;
    unsigned int j;
    for (j = 0; j < entities.size(); j++) {
        if (!entities[j].alive)
            break;
    }

    // Broadcast player arrival
    Client client = { i, j, ip, port, true };
    sf::Packet broadcastPack;
    broadcastPack << ServerRequest::PLAYER_JOIN
                  << client.id;
    broadcast(broadcastPack);
    
    // Add client to clients and entities
    clients[i] = client;
    entities[client.entityId] = clientEnt;
    connections++;

    // Alert player connection was successful
    sf::Packet packet;
    packet << ServerRequest::CONNECT_REQUEST_RESULT
           << ConnectionResult::SUCCESS
           << client.id;
    socket.send(packet, ip, port);

    std::cout << "Server: Connection request from client accepted, ID: " << client.id << " ("
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

    std::cout << "Server: Disconnect request from player: " << id << std::endl;

    // Mark player disconnected
    clients[id].connected = false;
    connections--;

    // Broadcast player disconnect
    sf::Packet broadcastPack;
    broadcastPack << ServerRequest::PLAYER_LEAVE << id;
    broadcast(broadcastPack);
}

void Server::broadcastWorld() {
    // 1. Send out player info
    // Start with just position, but later we'll want dir
    sf::Packet entityPacket;
    entityPacket << ServerRequest::ENTITY_UPDATE << connections;
    for (int i = 0; i < clients.size(); i++) {
        Entity e = entities[i];
        if (!clients[i].connected)
            continue;
        entityPacket << i << e.position.x << e.position.y << e.position.z;
    }
    broadcast(entityPacket);

    // 2. Send out world updates
    // TODO
}

void Server::handleUpdatePosition(sf::Packet p) {
    int clientId;
    float x, y, z;
    p >> clientId >> x >> y >> z;
    Client* c = &clients[clientId];
    int clientEntId = c->entityId;
    Entity* e = &entities[clientEntId];
    e->position = glm::vec3(x, y, z);
    std::cout << x << y << z << std::endl;
}
