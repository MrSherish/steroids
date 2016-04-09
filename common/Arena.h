#ifndef STEROIDS_WORLD_H
#define STEROIDS_WORLD_H


#include "Snake.h"

struct Arena {
    int width = 0;
    int height = 0;
    std::vector<Snake> snakes;

    Arena(int width = 0, int height = 0) : width(width), height(height) {}
};


#endif //STEROIDS_WORLD_H
