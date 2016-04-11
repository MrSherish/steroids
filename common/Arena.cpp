#include "Arena.h"
#include <random>

void Arena::fruitsOfLabor() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> numberOf(5, 20);
    std::uniform_int_distribution<> posX(0, width);
    std::uniform_int_distribution<> posY(0, height);
    int numberOfFruits = numberOf(gen);
    vec2 pos;
    for (int i = 0; i < numberOfFruits; i++) {
        bool flag = true;
        while (flag) {
            pos = vec2(posX(gen), posY(gen));
            flag = false;
            for (Fruit f : fruits) {
                if (f.pos == pos) flag = true;
            }
        }
        fruits.push_back(Fruit(1, pos));
    }
}
