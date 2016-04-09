#ifndef STEROIDS_COLOR_H
#define STEROIDS_COLOR_H


#include <cstdint>

struct Color {
    uint8_t r = 0, g = 0, b = 0, a = 255;

    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {}
};


#endif //STEROIDS_COLOR_H
