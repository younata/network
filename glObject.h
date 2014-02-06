#ifndef __GLOBJECT_H__
#define __GLOBJECT_H__

#ifdef __APPLE__
#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSIONS_INCLUDED
#include <OpenGL/gl3.h>
#include <OpenGL/glut.h>
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
    GLuint vertexBufferObject;
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
