#ifndef __network_tail__
#define __network_tail__ 1

#include "glObject.h"
#include "graphics.h"

class Tail : GLObject {
public:
    point3d center;
    point3d start; // this is a single point...
    double size;
    double color[3];

    // this uses the opengl immediate mode calls.
    void render(double fade=1.0);
    // this uses buffers (passed in, assumed to be of at least size 36)
    void render(float *vtx, float *col, double fade=1.0);

    // GL 3+
    void render3(double fade=1.0);

    Tail();
    Tail(point3d center, point3d start, double r, double g, double b);

    ~Tail();
};

#endif
