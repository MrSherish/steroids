#include "Snake.h"

Snake::Snake(vec2 pos, vec2 dir, Color color) : dir(dir), color(color) {
    segments.push_back(Segment{pos, false});
}

void Snake::proceed(int arenaWidth, int arenaHeight) {
    Segment &fs = segments.front();
    Segment &bs = segments.back();

    vec2 fpos = fs.pos + dir;
    if (fpos.x < 0) fpos.x = arenaWidth + fpos.x;
    if (fpos.y < 0) fpos.y = arenaHeight + fpos.y;
    fpos.x = fpos.x % arenaWidth;
    fpos.y = fpos.y % arenaHeight;

    segments.push_front(Segment{fpos, false});

    if (bs.fat) {
        bs.fat = false;
    } else {
        segments.pop_back();
    }
}
