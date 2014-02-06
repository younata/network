#ifndef __network_square__
#define __network_square__ 1

#include "glObject.h"
#include "graphics.h" // this is probably bad practice...

void assignBuffer(float *vtx, float x, float y, float z);

class Cube : GLObject {
public:
    point3d size;
    point3d center;
    double color[3];

    // this uses the opengl immediate mode calls.
    void render(double fade=1.0);
    // this uses buffers (which are passed in and assumed to be at least size 108)
    void render(float *vtx, float *col, double fade=1.0);

    // GL 3+
    void render3(double fade=1.0);

    Cube();

    Cube(point3d size, point3d center, double r, double g, double b);
    Cube(double s, point3d center, double r, double g, double b);

    ~Cube();
};

#endif
