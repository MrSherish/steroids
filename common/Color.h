#ifndef STEROIDS_COLOR_H
#define STEROIDS_COLOR_H


#include <cstdint>

struct Color {
    uint8_t r = 0, g = 0, b = 0, a = 255;

    Color(uint8_t r = 255, uint8_t g = 255, uint8_t b = 255, uint8_t a = 255) : r(r), g(g), b(b), a(a) {}
};


#endif //STEROIDS_COLOR_H
