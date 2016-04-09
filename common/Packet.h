#ifndef STEROIDS_PACKET_H
#define STEROIDS_PACKET_H

#include "../common/json.hpp"

#include <cstdint>

struct Packet {
    uint32_t ip = 0;
    uint16_t port = 0;
    nlohmann::json data;

    Packet(uint32_t ip = 0, uint16_t port = 0, nlohmann::json data = {}) : ip(ip), port(port), data(data) {}
};

#endif //STEROIDS_PACKET_H
