#include "Arena.h"
#include <random>

void Arena::createFruitsOnArena() {
    for (int i = 0; i < STARTING_NUMBER_OF_FRUITS; i++) {
        spawnNewFruit();
    }
}

bool Arena::canPlaceFruit(vec2 pos) {
    for (auto &f : fruits) {
        if (f.pos == pos) return false;
    }
    for (auto &s : snakes) {
        for (auto &a : s.segments) {
            if (a.pos == pos) return false;
        }
    }
    return true;
}

void Arena::spawnNewFruit() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> posX(0, width-1);
    std::uniform_int_distribution<> posY(0, height-1);
    std::uniform_int_distribution<> type(1, 3);
    vec2 pos;
    do {
        pos = vec2(posX(gen), posY(gen));
    } while (!canPlaceFruit(pos));
    std::cerr << "Spawned fruit " << pos.x << ":" << pos.y << std::endl;
    fruits.push_back(Fruit(type(gen), pos));
}