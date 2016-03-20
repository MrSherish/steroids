#ifndef STEROIDS_PACKET_H
#define STEROIDS_PACKET_H

#include "../common/json.hpp"

#include <cstdint>

struct Packet {
    uint32_t ip;
    uint16_t port;
    nlohmann::json data;
};

#endif //STEROIDS_PACKET_H
