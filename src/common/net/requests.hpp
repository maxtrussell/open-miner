#pragma once

using REQUEST_TYPE = uint8_t;

enum class CLIENT_REQUEST_TYPE : REQUEST_TYPE {
   CONNECT,
   DISCONNECT,
   KEYINPUT,
}; 

enum class SERVER_REQUEST_TYPE : REQUEST_TYPE {
    CONNECT_REQUEST_RESULT,
    PLAYER_JOIN,
    PLAYER_LEAVE,
    WORLD_STATE,
}; 

enum class CONNECTION_RESULT : REQUEST_TYPE {
    SUCCESS = 0,
    GAME_FULL = 1,
    DUPLICATE_ID = 2,
};

template <typename T>
sf::Packet &operator <<(sf::Packet &packet, const T &req) {
    return packet << static_cast<REQUEST_TYPE>(req);
}

template <typename T>
sf::Packet &operator >>(sf::Packet &packet, T &req) {
    REQUEST_TYPE reqId;
    packet >> reqId;
    req = static_cast<T>(reqId);
    return packet;
}
