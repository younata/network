#include "tail.h"
#include "cube.h" // for assignBuffer()

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <GLUT/glut.h>
#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSIONS_INCLUDED
#else
#include <GL/glut.h>
#include <GL/gl3.h>
#endif

Tail::Tail() : GLObject()
{
    verts = (float *)malloc(36 * sizeof(float));
    colors = (float *)malloc(36 * sizeof(float));
}

Tail::Tail(point3d center, point3d start, double r, double g, double b) :
center(center),
start(start)
{
    color[0] = r;
    color[1] = g;
    color[2] = b;
}

Tail::~Tail()
{
    free(verts);
    free(colors);
}

void Tail::render(double fade)
{
    double d = size / 2.0;
    
    glBegin(GL_TRIANGLES);
        glColor3f(color[0] * fade, color[1] * fade, color[2] * fade);
        glVertex3f(center.x - d, center.y - d, center.z + d);
        glVertex3f(center.x, center.y - d, center.z + d);
        glColor4f(1.0, 1.0, 1.0, fade);
        glVertex3f(start.x, start.y, start.z);
    glEnd();

    glBegin(GL_TRIANGLES);
        glColor3f(color[0] * fade, color[1] * fade, color[2] * fade);
        glVertex3f(center.x + d, center.y - d, center.z + d);
        glVertex3f(center.x, center.y - d, center.z + d);
        glColor4f(1.0, 1.0, 1.0, fade);
        glVertex3f(start.x, start.y, start.z);
    glEnd();

    glBegin(GL_TRIANGLES);
        glColor3f(color[0] * fade, color[1] * fade, color[2] * fade);
        glVertex3f(center.x + d, center.y - d, center.z - d);
        glVertex3f(center.x, center.y - d, center.z - d);
        glColor4f(1.0, 1.0, 1.0, fade);
        glVertex3f(start.x, start.y, start.z);
    glEnd();

    glBegin(GL_TRIANGLES);
        glColor3f(color[0] * fade, color[1] * fade, color[2] * fade);
        glVertex3f(center.x - d, center.y - d, center.z - d);
        glVertex3f(center.x, center.y - d, center.z - d);
        glColor4f(1.0, 1.0, 1.0, fade);
        glVertex3f(start.x, start.y, start.z);
    glEnd();
}

void Tail::render(float *vtx, float *col, double fade)
{
    double d = size;

    for (int i = 0; i < 12; i++) {
        assignBuffer((col+(3*i)), color[0] * fade, color[1] * fade, color[2] * fade);
        assignBuffer((col+(3*i)+1), color[0] * fade, color[1] * fade, color[2] * fade);
        assignBuffer((col+(3*i)+2), fade, fade, fade);
    }

    assignBuffer((vtx+0), center.x - d, center.y - d, center.z + d);
    assignBuffer((vtx+3), center.x, center.y - d, center.z + d);
    assignBuffer((vtx+6), start.x, start.y, start.z);

    assignBuffer((vtx+9), center.x + d, center.y - d, center.z + d);
    assignBuffer((vtx+12), center.x, center.y - d, center.z + d);
    assignBuffer((vtx+15), start.x, start.y, start.z);

    assignBuffer((vtx+18), center.x + d, center.y - d, center.z - d);
    assignBuffer((vtx+21), center.x, center.y - d, center.z - d);
    assignBuffer((vtx+24), start.x, start.y, start.z);

    assignBuffer((vtx+27), center.x - d, center.y - d, center.z - d);
    assignBuffer((vtx+30), center.x, center.y - d, center.z - d);
    assignBuffer((vtx+33), start.x, start.y, start.z);
}

void Tail::render3(double fade)
{
    render(verts, colors, fade);
    GLObject::render();
}
