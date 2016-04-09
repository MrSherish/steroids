#ifndef STEROIDS_SNAKE_H
#define STEROIDS_SNAKE_H


#include <vector>
#include <deque>

#include "vec2.h"
#include "Color.h"

struct Snake {
    struct Segment {
        vec2 pos;
        bool fat = false;

        Segment(vec2 pos, bool fat) : pos(pos), fat(fat) {}
    };

    std::deque<Segment> segments;
    vec2 dir;
    Color color;

    Snake(vec2 pos, vec2 dir, Color color);

    void proceed(int arenaWidth, int arenaHeight);
};


#endif //STEROIDS_SNAKE_H
