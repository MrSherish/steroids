#ifndef STEROIDS_COLOR_H
#define STEROIDS_COLOR_H


#include <cstdint>
#include "json.hpp"

struct Color {
    uint8_t r = 0, g = 0, b = 0, a = 255;

    Color(uint8_t r = 255, uint8_t g = 255, uint8_t b = 255, uint8_t a = 255) : r(r), g(g), b(b), a(a) {}

    nlohmann::json toJson() {
        std::vector<int> vec {r, g, b};
        return vec;
    }

    static Color fromJson(nlohmann::json jc) {
        uint8_t r = jc[0];
        uint8_t g = jc[1];
        uint8_t b = jc[2];
        return {r, g, b};
    }
};


#endif //STEROIDS_COLOR_H
