#ifndef __GLOBJECT_H__
#define __GLOBJECT_H__

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/gl3.h>
#endif

struct point3d {
    double x;
    double y;
    double z;
};

class GLObject {
protected:
    GLuint vertexArrayObject;
    GLuint vertexBufferObject[2];
    bool isGL3;
    float *verts;
    float *colors;
public:
    GLObject();

    void generateBuffers();
    void render();

    ~GLObject();
};

#endif
