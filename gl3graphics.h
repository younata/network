#ifndef __network_graphics__
#define __network_graphics__ 1

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl3.h>
#else
#include <GL/glut.h>
#include <GL/gl3.h>
#endif

#include <math.h>

#include "main.h"
#include "net.h"

#include "cube.h"
#include "tail.h"
#include <list>

struct cubeTail {
    struct point3d start;
    struct point3d dest;
    struct point3d curr;

    double speed

    int cyclesToKeepAround;

    double color[3];

    Cube cube;
    Tail tail;
};

class graphics {
private:
    GLuint vtxShader, frgShader;
    GLuint shaderprogram;

    GLuint vertexArrayObject;
    GLuint vertexBufferObject[2];

    std::list<cubeTail> cubes;

public:
    graphics();

    void generateBuffers();

    void tick();

    ~graphics();
};

#endif
