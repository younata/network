#include "graphics.h"

#include <assert.h>
#include <unistd.h>

#include <algorithm>

#include "cube.h"

extern pthread_mutex_t networkPacketsMutex;

struct particle {
    struct point3d start;
    struct point3d dest;
    struct point3d curr;

    double speed;

    int cyclesToKeepAround;
    
    double color[3];
};

std::vector<packet> *packets;

std::vector<particle> *particles;

int totalCycles = 30;

double maxWidth = 10.0;
double particleWidth = 0.025;

struct point3d windowPos;
struct point3d windowSize;
bool isFullScreen = false;

bool is3d = false;

struct point3d calculatePosition(unsigned char addr[], double height, double width)
{
    struct point3d ret;
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

    ret.x = (((double)x / (maxheight)) * 2) - 1;
    if (is3d) {
        ret.z = (((double)y / (maxheight)) * 2) - 2;
        ret.y = 1;
    } else {
        ret.y = (((double)y / (maxheight)) * 2) - 1;
        ret.z = 0;
    }

    return ret;
}

struct point3d calculateCurrentPosition(struct point3d curr, struct point3d dest, double speed)
{
    struct point3d ret;
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

struct point3d calculateCurrentPosition3d(struct point3d curr, struct point3d dest, struct point3d start, double speed)
{
    struct point3d ret;
    double x,y,z;
    // cheat, because I know it'll take 30 ticks before it gets there...
    x = (dest.x - start.x) / 30.0;
    y = (dest.y - start.y) / 30.0;
    z = (dest.z - start.z) / 30.0;
    double dist = sqrt(pow(curr.x - dest.x, 2) + pow(curr.y - dest.y, 2) + pow(curr.z - dest.z, 2));
    double dt = dist - speed;
    if (dt <= 0) {
        return dest;
    }
    ret.x = curr.x + x; ret.y = curr.y + y; ret.z = curr.z + z;
    return ret;
}

bool sortParticle(particle a, particle b) { return (a.cyclesToKeepAround < b.cyclesToKeepAround); }

bool sortParticle3d(particle a, particle b)
{
    bool aIsOlder = sortParticle(a, b);
    return aIsOlder && fabs(a.curr.z) > fabs(b.curr.z);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    float vert[3];
    double d = particleWidth / 2.0;
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_SMOOTH);
    glDepthMask(false);

/* Ideally, I'd like to display the planes that the particles are coming from, but it's not working for me, so this'll be commented out.
    if (is3d) {
        point3d size, center;
        size.x = 2.0; size.y = 1; size.z = 2.0;
        center.x = 0.0; center.y = 1.0; center.z = 1.0;

        Cube top = Cube(size, center, 1.0, 1.0, 1.0);

        center.y = -1.0;
        Cube bottom = Cube(size, center, 1.0, 1.0, 1.0);

        top.render(0.0);
        bottom.render(0.0);
    }
*/
    
    for (std::vector<particle>::iterator i = particles->begin(); i < particles->end(); i++) {
        particle p = *i;
        double fade = (double)p.cyclesToKeepAround / (double)totalCycles;
        if (!is3d) {
            glBegin(GL_QUADS);
                glColor3f(p.color[0] * fade, p.color[1] * fade, p.color[2] * fade);
                glVertex2f(p.curr.x - d, p.curr.y - d);
                glVertex2f(p.curr.x + d, p.curr.y - d);
                glVertex2f(p.curr.x + d, p.curr.y + d);
                glVertex2f(p.curr.x - d, p.curr.y + d);
            glEnd();

            glBegin(GL_TRIANGLES);
                glColor3f(p.color[0] * fade, p.color[1] * fade, p.color[2] * fade);
                //glColor4f(p.color[0], p.color[1], p.color[2], fade);
                glVertex2f(p.curr.x - d, p.curr.y);
                glVertex2f(p.curr.x + d, p.curr.y);
                glColor4f(1.0, 1.0, 1.0, fade);
                glVertex2f(p.start.x, p.start.y);
            glEnd();
        } else {
            Cube c = Cube(d, p.curr, p.color[0], p.color[1], p.color[2]);
            c.render(fade);

            // drawing the tail!
            glBegin(GL_TRIANGLES);
                glColor3f(p.color[0] * fade, p.color[1] * fade, p.color[2] * fade);
                glVertex3f(p.curr.x - d, p.curr.y - d, p.curr.z + d);
                glVertex3f(p.curr.x, p.curr.y - d, p.curr.z + d);
                glColor4f(1.0, 1.0, 1.0, fade);
                glVertex3f(p.start.x, p.start.y, p.start.z);
            glEnd();

            glBegin(GL_TRIANGLES);
                glColor3f(p.color[0] * fade, p.color[1] * fade, p.color[2] * fade);
                glVertex3f(p.curr.x + d, p.curr.y - d, p.curr.z + d);
                glVertex3f(p.curr.x, p.curr.y - d, p.curr.z + d);
                glColor4f(1.0, 1.0, 1.0, fade);
                glVertex3f(p.start.x, p.start.y, p.start.z);
            glEnd();

            glBegin(GL_TRIANGLES);
                glColor3f(p.color[0] * fade, p.color[1] * fade, p.color[2] * fade);
                glVertex3f(p.curr.x + d, p.curr.y - d, p.curr.z - d);
                glVertex3f(p.curr.x, p.curr.y - d, p.curr.z - d);
                glColor4f(1.0, 1.0, 1.0, fade);
                glVertex3f(p.start.x, p.start.y, p.start.z);
            glEnd();

            glBegin(GL_TRIANGLES);
                glColor3f(p.color[0] * fade, p.color[1] * fade, p.color[2] * fade);
                glVertex3f(p.curr.x - d, p.curr.y - d, p.curr.z - d);
                glVertex3f(p.curr.x, p.curr.y - d, p.curr.z - d);
                glColor4f(1.0, 1.0, 1.0, fade);
                glVertex3f(p.start.x, p.start.y, p.start.z);
            glEnd();
        }
    }
    glDepthMask(true);
    glFlush();
    //glutPostRedisplay();
}

void idleFrame(int i)
{
    idle();
}

void idle()
{
    int amount=3;
    while (packets->size() != 0 && amount > 0) {
        amount--;
        pthread_mutex_lock(&networkPacketsMutex);
        packet pkt = packets->back();
        particle p;
        p.cyclesToKeepAround = totalCycles;
        p.start = calculatePosition(pkt.sourceAddr, 2, 2);
        p.dest = calculatePosition(pkt.destAddr, 2, 2);
        if (is3d)
            p.dest.y = -1;
        p.curr = p.start;

        if (!is3d)
            p.speed = sqrt(pow(p.curr.x - p.dest.x, 2) + pow(p.curr.y - p.dest.y, 2)) / 30;
        else
            p.speed = sqrt(pow(p.curr.x - p.dest.x, 2) + pow(p.curr.y - p.dest.y, 2) + pow(p.curr.z - p.dest.z, 2)) / 30;

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
        if (p.speed > 0)
            particles->push_back(p);
    }
    for (std::vector<particle>::iterator i = particles->begin(); i < particles->end(); i++) {
        particle p = *i;
        bool keep = true;
        if (p.curr.x == p.dest.x && p.curr.y == p.dest.y) {
            p.cyclesToKeepAround--;
            if (p.cyclesToKeepAround <= 0) {
                keep = false;
                particles->erase(i);
            }
        }
        if (keep) {
            if (!is3d)
                p.curr = calculateCurrentPosition(p.curr, p.dest, p.speed);
            else
                p.curr = calculateCurrentPosition3d(p.curr, p.dest, p.start, p.speed);
            particles->erase(i);
            particles->insert(i, p);
        }
    }
    if (!is3d)
        std::sort(particles->begin(), particles->end(), sortParticle);
    else
        std::sort(particles->begin(), particles->end(), sortParticle3d);
    display();
    glutTimerFunc(33, idleFrame, 0);
}

void changeSize(int w, int h)
{
	if (h == 0)
		h = 1;
	float ratio = 1.0* w / h;

    if (w > h) {
        particleWidth = 8.0 / h;
    } else {
        particleWidth = 8.0 / w;
    }

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(0,ratio,1,1000);
	glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 'f':
            if (!isFullScreen) {
                windowPos.x = (double)glutGet(GLUT_WINDOW_X);
                windowPos.y = (double)glutGet(GLUT_WINDOW_Y);
                windowSize.x = (double)glutGet(GLUT_WINDOW_WIDTH);
                windowSize.y = (double)glutGet(GLUT_WINDOW_HEIGHT);
                isFullScreen = true;
                glutFullScreen();
            } else {
                glutReshapeWindow((int)windowSize.x, (int)windowSize.y);
                glutPositionWindow((int)windowPos.x, (int)windowPos.y);
                glutSetWindowTitle("NetworkMonitor");
                glutSetIconTitle("NetworkMonitor");
                isFullScreen = false;
            }
            break;
    }
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glColor3f(0.0, 0.0, 0.0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_MULTISAMPLE);
}

void initGL(std::vector<packet> *p, int argc, char *argv[], bool use3d)
{
    packets = p;
    particles = new std::vector<particle>();

    is3d = use3d;

    glutInit(&argc, argv);

    isFullScreen = false;

    glutCreateWindow("NetworkMonitor");
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_SINGLE | GLUT_DOUBLE | GLUT_MULTISAMPLE);
    glutInitWindowPosition(400,400);
    glutInitWindowSize(800,800);
    glutDisplayFunc(display);
    glutReshapeFunc(changeSize);
    glutKeyboardFunc(keyboard);
    particleWidth = 8.0 / 800.0;
    init();
    glutIdleFunc(NULL);
    glutTimerFunc(33, idleFrame, 0);
    /*
    pthread_t glThread;
    pthread_create(&glThread, NULL, runGLThread, NULL);
    */
    glutMainLoop();

    //pthread_join(glThread, NULL);
}
