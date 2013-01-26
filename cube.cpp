#include "cube.h"

Cube::Cube(point3d size, point3d center, double r, double g, double b): size(size), center(center)
{
    color[0] = r;
    color[1] = g;
    color[2] = b;
}

Cube::Cube(double s, point3d center, double r, double g, double b): center(center)
{
    size.x = s;
    size.y = s;
    size.z = s;
}


void Cube::render(double fade)
{
    double x = size.x / 2.0;
    double y = size.y / 2.0;
    double z = size.z / 2.0;
    // front square
    glBegin(GL_TRIANGLES);
        glColor3f(color[0] * fade, color[1] * fade, color[2] * fade);
        glVertex3f(center.x - x, center.y - y, center.z + z);
        glVertex3f(center.x - x, center.y + y, center.z + z);
        glVertex3f(center.x + x, center.y + y, center.z + z);
    glEnd();

    glBegin(GL_TRIANGLES);
        glColor3f(color[0] * fade, color[1] * fade, color[2] * fade);
        glVertex3f(center.x - x, center.y - y, center.z + z);
        glVertex3f(center.x + x, center.y - y, center.z + z);
        glVertex3f(center.x + x, center.y + y, center.z + z);
    glEnd();

    // right side square
    glBegin(GL_TRIANGLES);
        glColor3f(color[0] * fade, color[1] * fade, color[2] * fade);
        glVertex3f(center.x - x, center.y - y, center.z + z);
        glVertex3f(center.x - x, center.y + y, center.z + z);
        glVertex3f(center.x - x, center.y - y, center.z - z);
    glEnd();

    glBegin(GL_TRIANGLES);
        glColor3f(color[0] * fade, color[1] * fade, color[2] * fade);
        glVertex3f(center.x - x, center.y + y, center.z + z);
        glVertex3f(center.x - x, center.y - y, center.z + z);
        glVertex3f(center.x - x, center.y + y, center.z - z);
    glEnd();

    // left side square
    glBegin(GL_TRIANGLES);
        glColor3f(color[0] * fade, color[1] * fade, color[2] * fade);
        glVertex3f(center.x + x, center.y - y, center.z + z);
        glVertex3f(center.x + x, center.y + y, center.z + z);
        glVertex3f(center.x + x, center.y - y, center.z - z);
    glEnd();

    glBegin(GL_TRIANGLES);
        glColor3f(color[0] * fade, color[1] * fade, color[2] * fade);
        glVertex3f(center.x + x, center.y + y, center.z + z);
        glVertex3f(center.x + x, center.y - y, center.z + z);
        glVertex3f(center.x + x, center.y + y, center.z - z);
    glEnd();

    // top side square
    glBegin(GL_TRIANGLES);
        glColor3f(color[0] * fade, color[1] * fade, color[2] * fade);
        glVertex3f(center.x - x, center.y + y, center.z + z);
        glVertex3f(center.x + x, center.y + y, center.z + z);
        glVertex3f(center.x - x, center.y + y, center.z - z);
    glEnd();

    glBegin(GL_TRIANGLES);
        glColor3f(color[0] * fade, color[1] * fade, color[2] * fade);
        glVertex3f(center.x - x, center.y + y, center.z - z);
        glVertex3f(center.x + x, center.y + y, center.z - z);
        glVertex3f(center.x + x, center.y + y, center.z + z);
    glEnd();

    // bottom side square
    glBegin(GL_TRIANGLES);
        glColor3f(color[0] * fade, color[1] * fade, color[2] * fade);
        glVertex3f(center.x - x, center.y - y, center.z + z);
        glVertex3f(center.x + x, center.y - y, center.z + z);
        glVertex3f(center.x - x, center.y - y, center.z - z);
    glEnd();

    glBegin(GL_TRIANGLES);
        glColor3f(color[0] * fade, color[1] * fade, color[2] * fade);
        glVertex3f(center.x - x, center.y - y, center.z - z);
        glVertex3f(center.x + x, center.y - y, center.z - z);
        glVertex3f(center.x + x, center.y - y, center.z + z);
    glEnd();

    // back side square
    glBegin(GL_TRIANGLES);
        glColor3f(color[0] * fade, color[1] * fade, color[2] * fade);
        glVertex3f(center.x - x, center.y - y, center.z - z);
        glVertex3f(center.x - x, center.y + y, center.z - z);
        glVertex3f(center.x + x, center.y + y, center.z - z);
    glEnd();

    glBegin(GL_TRIANGLES);
        glColor3f(color[0] * fade, color[1] * fade, color[2] * fade);
        glVertex3f(center.x - x, center.y - y, center.z - z);
        glVertex3f(center.x + x, center.y - y, center.z - z);
        glVertex3f(center.x + x, center.y + y, center.z - z);
    glEnd();
}

