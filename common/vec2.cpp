#include "vec2.h"

vec2 vec2::operator+(vec2 o) {
    return {x + o.x, y + o.y};
}

bool vec2::operator==(vec2 o) {
    return x == o.x && y == o.y;
}

bool vec2::operator!=(vec2 o) {
    return !(*this == o);
}

nlohmann::json vec2::toJson() const {
    return std::vector<int> {x, y};
}

vec2 vec2::fromJson(nlohmann::json j) {
    return {j[0], j[1]};
}