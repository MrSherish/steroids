#ifndef STEROIDS_PLAYER_H
#define STEROIDS_PLAYER_H

#include <string>

struct Player {
    int playerId = 0;
    uint32_t ip = 0;
    uint16_t port = 0;
    std::string nick;
	int points = 0;
};


#endif //STEROIDS_PLAYER_H
