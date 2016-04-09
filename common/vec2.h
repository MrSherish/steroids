#ifndef STEROIDS_VEC2_H
#define STEROIDS_VEC2_H


struct vec2 {
    int x = 0;
    int y = 0;

    vec2(int x = 0, int y = 0) : x(x), y(y) {}

    vec2 operator+(vec2 o);
};


#endif //STEROIDS_VEC2_H
