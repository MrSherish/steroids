#include "vec2.h"

vec2 vec2::operator+(vec2 o) {
    return {x + o.x, y + o.y};
}