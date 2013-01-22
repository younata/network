#include "graphics.h"

#include <assert.h>

extern pthread_mutex_t networkPacketsMutex;

struct particle {
    struct point2d start;
    struct point2d dest;
    struct point2d curr;
    
    double color[3];
};

std::vector<packet> *packets;

std::vector<particle> *particles;

//double maxHeight = 5.0;
double maxWidth = 10.0;
double particleWidth = 0.025;

struct point2d calculatePosition(unsigned char addr[], double height, double width)
{
    struct point2d ret;
    unsigned char cws;

    unsigned int x = 0, y = 0;


    double maxheight = 65535.0;

    for (int i = 0; i < 4; i++) {
        cws = addr[i];

        x += (cws & 0xf) << (4 * (3-i));
        y += (cws >> 4) << (4 * (3-i));
    }
    assert(x < maxheight);
    assert(y < maxheight);

    ret.x = ((double)x / (maxheight));// * width;
    ret.y = ((double)y / (maxheight));// * height;

    return ret;
}

struct point2d calculateCurrentPosition(struct point2d curr, struct point2d dest, double speed)
{
    struct point2d ret;
    double x,y;
    x = dest.x - curr.x;
    y = dest.y - curr.y;
    double dist = sqrt(x*x + y*y);
    double dt = dist - speed;
    if (dt < 0) {
        return dest;
    }
    double angle = atan2(y, x);
    x = dt * cos(angle);
    y = dt * sin(angle);
    ret.x = dest.x - x;
    ret.y = dest.y - y;
    return ret;
}

void idle()
{
    while (packets->size() != 0) {
        pthread_mutex_lock(&networkPacketsMutex);
        packet pkt = packets->back();
        particle p;
        p.start = calculatePosition(pkt.sourceAddr, maxWidth, maxWidth);
        p.dest = calculatePosition(pkt.destAddr, maxWidth, maxWidth);
        p.curr.x = p.start.x; p.curr.y = p.start.y;
        switch (pkt.type) {
            case 0:
                p.color[0] = 0.0;
                p.color[1] = 0.5;
                p.color[2] = 1.0;
                break;
            case 1:
                p.color[0] = 0.0;
                p.color[1] = 1.0;
                p.color[2] = 0.0;
                break;
            case 2:
                p.color[0] = 0.0;
                p.color[1] = 0.0;
                p.color[2] = 1.0;
                break;
            case 3:
                p.color[0] = 0.0;
                p.color[1] = 1.0;
                p.color[2] = 0.5;
                break;
            case 4:
                p.color[0] = 1.0;
                p.color[1] = 0.0;
                p.color[2] = 0.0;
                break;
        }
        packets->pop_back();
        pthread_mutex_unlock(&networkPacketsMutex);
        particles->push_back(p);
    }
    for (std::vector<particle>::iterator i = particles->begin(); i < particles->end(); i++) {
        particle p = *i;
        if (p.curr.x == p.dest.x && p.curr.y == p.dest.y) {
            particles->erase(i);
        }
        point2d c; c.x = p.curr.x; c.y = p.curr.y;
        point2d a = calculateCurrentPosition(p.curr, p.dest, 0.5);
        p.curr.x = a.x; p.curr.y = a.y;
        //assert(c.x != p.curr.x && c.y != p.curr.y);
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    float vert[3];
    double d = particleWidth / 2.0;
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_SMOOTH);
    for (std::vector<particle>::iterator i = particles->begin(); i < particles->end(); i++) {
        particle p = *i;
///*
        glBegin(GL_QUADS);
            glColor3f(p.color[0], p.color[1], p.color[2]);
            glVertex2f(p.curr.x - d, p.curr.y - d);
            glVertex2f(p.curr.x + d, p.curr.y - d);
            glVertex2f(p.curr.x + d, p.curr.y + d);
            glVertex2f(p.curr.x - d, p.curr.y + d);
        glEnd();
//*/
/*
        glBegin(GL_TRIANGLES);
            glColor3f(p.color[0], p.color[1], p.color[2]);
            vert[0] = p.curr.x - (particleWidth / 2.0);
            vert[1] = p.curr.y - (particleWidth / 2.0);
            vert[2] = 0;
            glVertex3fv(vert);

            glColor3f(p.color[0], p.color[1], p.color[2]);
            vert[0] = p.curr.x + (particleWidth / 2.0);
            glVertex3fv(vert);

            glColor3f(p.color[0], p.color[1], p.color[2]);
            vert[0] = p.curr.x;
            vert[1] = p.curr.y + (particleWidth / 2.0);
            glVertex3fv(vert);
        glEnd();
//*/
    }
    glFlush();
    glutPostRedisplay();
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glColor3f(0.0, 0.0, 0.0);

    glEnable(GL_DEPTH_TEST);
/*
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 10.0, 10.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
*/
}

void initGL(std::vector<packet> *p, int argc, char *argv[])
{
    packets = p;
    particles = new std::vector<particle>();

    glutInit(&argc, argv);
    
    glutCreateWindow("NetworkMonitor");
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_SINGLE);
    glutInitWindowPosition(400,400);
    glutInitWindowSize(800,800);
    glutDisplayFunc(display);
    init();
    glutIdleFunc(idle);
    glutMainLoop();
}
