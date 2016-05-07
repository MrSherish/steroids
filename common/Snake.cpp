#include "Snake.h"

using nlohmann::json;

Snake::Snake(vec2 pos, vec2 dir, Color color) : dir(dir), color(color) {
    segments.push_back(Segment{pos, false});
}

void Snake::proceed(int arenaWidth, int arenaHeight) {
    Segment &fs = segments.front();
    Segment &bs = segments.back();
    if (clock == 10) {
        clock = 0;
        bonus = Nope;
    }
    else if (clock > 0) {
        clock++;
    }

    vec2 fpos = fs.pos + dir;
    if (fpos.x < 0) fpos.x = arenaWidth + fpos.x;
    if (fpos.y < 0) fpos.y = arenaHeight + fpos.y;
    fpos.x = fpos.x % arenaWidth;
    fpos.y = fpos.y % arenaHeight;

    if (dir != vec2{0, 0}) {
        segments.push_front(Segment{fpos, false});

        if (bs.fat) {
            bs.fat = false;
        } else {
            segments.pop_back();
        }
    }
}

nlohmann::json Snake::toJson() const {
    std::vector<json> segs;
    for (auto seg : segments) {
        segs.push_back(seg.pos.toJson());
    }

    return {
            {"playerId", playerId},
            {"color", color.toJson()},
            {"segments", segs},
            {"isAlive", alive}
    };
}

Snake Snake::fromJson(nlohmann::json j) {
    Snake snake;
    snake.playerId = j["playerId"];
    snake.color = Color::fromJson(j["color"]);

    std::deque<Segment> segs;
    for (json sj : j["segments"]) {
        Segment seg {vec2::fromJson(sj)};
        segs.push_back(seg);
    }
    snake.segments = segs;
    snake.alive = j["isAlive"];

    return snake;
}

void Snake::getBonus(const Fruit &f) {
    if (f.type == 1) {
        bonus = Greedy$nake;
        clock = 1;
    }
}

bool Snake::isGreedy() {
    return bonus == Greedy$nake;
}



