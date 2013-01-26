#ifndef __network_square__
#define __network_square__ 1

#include "graphics.h" // this is probably bad practice...

class Cube {
public:
    point3d size;
    point3d center;
    double color[3];

    void render(double fade=1.0);

    Cube(point3d size, point3d center, double r, double g, double b);
    Cube(double s, point3d center, double r, double g, double b);
};

#endif