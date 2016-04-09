#ifndef STEROIDS_WORLD_H
#define STEROIDS_WORLD_H


#include "Snake.h"
#include "Player.h"

struct Arena {
    int width = 0;
    int height = 0;
    std::vector<Snake> snakes;
    std::vector<Player> players;

    Arena(int width = 0, int height = 0) : width(width), height(height) {}
};


#endif //STEROIDS_WORLD_H
