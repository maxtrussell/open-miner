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

    // Start the game clock
    glfwInit();
}

void Server::run(sf::Time timeout) {
    while (running) {
        // Calculate deltaTime
        float currFrame = glfwGetTime();
        deltaTime = currFrame - lastFrame;
        lastFrame = currFrame;

        listen(deltaTime);
        // update(); TODO
        broadcastWorld();

        // Wait as necessary for tick
        tick(glfwGetTime() - currFrame);
    }
}

void Server::listen(float deltaTime) {
    // TODO: ensure to only accept one client move per tick
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
            case ClientRequest::KEY_INPUT:
                handleClientInput(req.packet, deltaTime);
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
    clientEnt.active = true;
    unsigned int j;
    for (j = 0; j < entities.size(); j++) {
        if (!entities[j].active)
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
    Client* c = &clients[id];
    c->connected = false;

    Entity* e = &entities[c->entityId];
    e->active = false;
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

void Server::handleClientInput(sf::Packet p, float deltaTime) {
    int clientId;
    Movement m;
    p >> clientId >> m;
    Client c = clients[clientId];
    Entity* e = &entities[c.entityId];
    e->handleMovement(m, deltaTime);
}

void Server::tick(float elapsedTime) {
    int sleepTime = (tickLength - elapsedTime) * 1000;  // milliseconds
    if (sleepTime > 0)
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
}
