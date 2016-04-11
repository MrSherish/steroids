#ifndef STEROIDS_FRUIT_H
#define STEROIDS_FRUIT_H


#include "vec2.h"

struct Fruit {
    vec2 pos;
    int type;

    Fruit(int type = 0, vec2 p = { 0,0 }) : pos(p), type(type) {}
};


#endif //STEROIDS_FRUIT_H
