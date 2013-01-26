#ifndef __network_graphics__
#define __network_graphics__ 1

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>

#include "main.h"
#include "net.h"

struct point3d {
    double x;
    double y;
    double z;
};

void initGL(std::vector<packet> *p, int argc, char *argv[], bool use3d);

struct point3d calculatePosition(unsigned char addr[], double height, double width);
struct point3d calculateCurrentPosition(struct point3d curr, struct point3d dest, double speed);
struct point3d calculateCurrentPosition3d(struct point3d curr, struct point3d dest, struct point3d start, double speed);

void idle();

#endif
