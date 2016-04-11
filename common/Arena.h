#ifndef STEROIDS_WORLD_H
#define STEROIDS_WORLD_H


#include "Snake.h"
#include "Player.h"
#include "Fruit.h"

const int STARTING_NUMBER_OF_FRUITS = 17;

struct Arena {  
    int width = 0;
    int height = 0;
    std::vector<Snake> snakes;
    std::vector<Player> players;
    std::vector<Fruit> fruits;

    Arena(int width = 0, int height = 0) : width(width), height(height) {}
    void createFruitsOnArena();
    void spawnNewFruit();
    bool canPlaceFruit(vec2 pos);
};


#endif //STEROIDS_WORLD_H
