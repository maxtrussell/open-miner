#pragma once

#include <cstdint>

enum class ClientRequest : uint8_t {
   CONNECT,
   DISCONNECT,
   KEY_INPUT,
}; 

enum class ServerRequest : uint8_t {
    CONNECT_REQUEST_RESULT,
    PLAYER_JOIN,
    PLAYER_LEAVE,
    ENTITY_UPDATE,
    WORLD_UPDATE,
}; 

enum class ConnectionResult : uint8_t {
    SUCCESS = 0,
    GAME_FULL = 1,
    DUPLICATE_ID = 2,
};

template <typename T>
sf::Packet &operator <<(sf::Packet &packet, const T &req) {
    return packet << static_cast<uint8_t>(req);
}

template <typename T>
sf::Packet &operator >>(sf::Packet &packet, T &req) {
    uint8_t reqId;
    packet >> reqId;
    req = static_cast<T>(reqId);
    return packet;
}
