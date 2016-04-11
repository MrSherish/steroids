#include "Arena.h"
#include <random>

const int STARTING_NUMBER_OF_FRUITS = 17;

void Arena::createFruitsOnArena() {
    for (int i = 0; i < STARTING_NUMBER_OF_FRUITS; i++) {
        spawnNewFruit();
    }
}

bool Arena::canPlaceFruit(vec2 pos) {
    for (Fruit &f : fruits) {
        if (f.pos == pos) return false;
    }
    for (Snake &s : snakes) {
        for (Snake::Segment &a : s.segments) {
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
    vec2 pos;
    do {
        pos = vec2(posX(gen), posY(gen));
    } while (!canPlaceFruit(pos));
    fruits.push_back(Fruit(1, pos));
}