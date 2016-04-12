#ifndef STEROIDS_PLAYER_H
#define STEROIDS_PLAYER_H

#include <string>
#include "json.hpp"

struct Player {
    int playerId = 0;
    uint32_t ip = 0;
    uint16_t port = 0;
    std::string nick;
    int points = 0;

    nlohmann::json toJson() const;

    static Player fromJson(nlohmann::json j);
};


#endif //STEROIDS_PLAYER_H
