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

struct point2d {
    double x;
    double y;
};

void initGL(std::vector<packet> *p, int argc, char *argv[]);

struct point2d calculatePosition(unsigned char addr[], double height, double width);
struct point2d calculateCurrentPosition(struct point2d curr, struct point2d dest, double speed);

void idle();

#endif
