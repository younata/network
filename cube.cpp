#include "cube.h"

void assignBuffer(float *vtx, float x, float y, float z)
{
    vtx[0] = x;
    vtx[1] = y;
    vtx[2] = z;
}

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

void Cube::render(float *vtx, float *col, double fade)
{
    // two arrays of at least size 108.
    double x = size.x;
    double y = size.y;
    double z = size.z;

    // first, colors...
    for (int i = 0; i < 36; i++) {
        for (int j = 0; j < 3; j++) {
            col[3*i+j] = color[j] * fade;
        }
    }
    // there has got to be a better way to do the following...
    // front square
    assignBuffer((vtx+0), center.x - x, center.y - y, center.z + z);
    assignBuffer((vtx+3), center.x - x, center.y + y, center.z + z);
    assignBuffer((vtx+6), center.x + x, center.y + y, center.z + z);

    vtx += 9;
    assignBuffer((vtx+0), center.x - x, center.y - y, center.z + z);
    assignBuffer((vtx+3), center.x + x, center.y - y, center.z + z);
    assignBuffer((vtx+6), center.x + x, center.y + y, center.z + z);

    vtx += 9;
    // right side square
    assignBuffer((vtx+0), center.x - x, center.y - y, center.z + z);
    assignBuffer((vtx+3), center.x - x, center.y + y, center.z + z);
    assignBuffer((vtx+6), center.x - x, center.y - y, center.z - z);

    vtx += 9;
    assignBuffer((vtx+0), center.x - x, center.y + y, center.z + z);
    assignBuffer((vtx+3), center.x - x, center.y - y, center.z + z);
    assignBuffer((vtx+6), center.x - x, center.y + y, center.z - z);

    vtx += 9;
    // left side square
    assignBuffer((vtx+0), center.x + x, center.y - y, center.z + z);
    assignBuffer((vtx+3), center.x + x, center.y + y, center.z + z);
    assignBuffer((vtx+6), center.x + x, center.y - y, center.z - z);

    vtx += 9;
    assignBuffer((vtx+0), center.x + x, center.y + y, center.z + z);
    assignBuffer((vtx+3), center.x + x, center.y - y, center.z + z);
    assignBuffer((vtx+6), center.x + x, center.y + y, center.z - z);

    vtx += 9;
    // top side square
    assignBuffer((vtx+0), center.x - x, center.y + y, center.z + z);
    assignBuffer((vtx+3), center.x + x, center.y + y, center.z + z);
    assignBuffer((vtx+6), center.x - x, center.y + y, center.z - z);

    vtx += 9;
    assignBuffer((vtx+0), center.x - x, center.y + y, center.z - z);
    assignBuffer((vtx+3), center.x + x, center.y + y, center.z - z);
    assignBuffer((vtx+6), center.x + x, center.y + y, center.z + z);

    vtx += 9;
    // bottom side square
    assignBuffer((vtx+0), center.x - x, center.y - y, center.z + z);
    assignBuffer((vtx+3), center.x + x, center.y - y, center.z + z);
    assignBuffer((vtx+6), center.x - x, center.y - y, center.z - z);

    vtx += 9;
    assignBuffer((vtx+0), center.x - x, center.y - y, center.z - z);
    assignBuffer((vtx+3), center.x + x, center.y - y, center.z - z);
    assignBuffer((vtx+6), center.x + x, center.y - y, center.z + z);

    vtx += 9;
    // back side square
    assignBuffer((vtx+0), center.x - x, center.y - y, center.z - z);
    assignBuffer((vtx+3), center.x - x, center.y + y, center.z - z);
    assignBuffer((vtx+6), center.x + x, center.y + y, center.z - z);

    vtx += 9;
    assignBuffer((vtx+0), center.x - x, center.y - y, center.z - z);
    assignBuffer((vtx+3), center.x + x, center.y - y, center.z - z);
    assignBuffer((vtx+6), center.x + x, center.y + y, center.z - z);
}
