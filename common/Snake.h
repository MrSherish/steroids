#ifndef STEROIDS_SNAKE_H
#define STEROIDS_SNAKE_H


#include <vector>
#include <deque>

#include "vec2.h"
#include "Color.h"
#include "Fruit.h"

struct Snake {
    enum bonuses {
        Greedy$nake,
        Sprint, Nope
    };
    struct Segment {
        vec2 pos;
        bool fat = false;

        Segment(vec2 pos, bool fat = false) : pos(pos), fat(fat) {}
    };

    int playerId = 0;
    bonuses bonus = Nope;
    int clock = 0;
    std::deque<Segment> segments;
    vec2 dir;
    Color color;
    bool alive = true;
    bool isDying = false;
    uint32_t deathTick = 0;

    Snake(vec2 pos, vec2 dir, Color color);

    Snake() = default;

    void proceed(int arenaWidth, int arenaHeight);

    void getBonus(const Fruit &f);

    bool isGreedy();

    nlohmann::json toJson() const;

    static Snake fromJson(nlohmann::json j);
};


#endif //STEROIDS_SNAKE_H
