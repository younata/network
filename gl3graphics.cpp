#include "graphics.h"

#include <assert.h>
#include <unistd.h>

#include <algorithm>

#include <time.h>
#include <list>

#include "cube.h"
#include "tail.h"

#include "gl3graphics.h"

extern pthread_mutex_t networkPacketsMutex;

std::list<packet> *packets;

int totalCycles = 30;

double maxWidth = 10.0;
double particleWidth = 0.025;

struct point3d windowPos;
struct point3d windowSize;
bool isFullScreen = false;

bool is3d = false;

struct point3d curMousePos;
char mouseAddr[18];


char *readFile(char *infile)
{
    FILE *f = fopen(infile, "r");
    if (!f)
        return NULL;
    fseek(f, 0, SEEK_END);
    size_t length = ftell(f);
    char *ret = (char *)malloc(length + 1);
    fseek(f, 0, SEEK_SET);
    fread(ret, length, 1, f);
    fclose(f);
    ret[length] = 0;
    return ret;
}

graphics::graphics()
{
    cubes = std::list<cube>();
}

graphics::generateBuffers(char *vrtFile, char *frgFile)
{
    char *vrtSource = readFile(vrtFile);
    char *frgSource = readFile(frgFile);
    
    vtxShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vtxShader, 1, (const GLchar**)&vrtSource, 0);
    glCompileShader(vtxShader);
    int isCompiled = 0;
    glGetShaderiv(vtxShader, GL_COMPILE_STATUS, &isCompiled);
    if (!isCompiled) {
        // FIXME
        exit(1);
    }

    frgShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frgShader, 1, (const GLchar**)&frgSource, 0);
    glCompileShader(frgShader);
    glGetShaderiv(frgShader, GL_COMPILE_STATUS, &isCompiled);
    if (!isCompiled) {
        // FIXME
        exit(1);
    }

    shaderprogram = glCreateProgram();
    glAttachShader(shaderprogram, vtxShader);
    glAttachShader(shaderprogram, frgShader);

    glBindAttribLocation(shaderprogram, 0, "in_Position");
    glBindAttribLocation(shaderprogram, 1, "in_Color");

    glLinkProgram(shaderprogram);

    int isLinked = 0;
    glGetProgramiv(shaderprogram, GL_LINK_STATUS, &isLinked);
    if (!isLinked) {
        // FIXME
        exit(2);
    }

    free(frgSource);
    free(vrtSource);
}

bool sortCubes(cubeTail a, cubeTail b) { return (a.cyclesToKeepAround < b.cyclesToKeepAround); }

void graphics::tick()
{
    while (packets->size() != 0 && amount > 0) {
        amount--;
        pthread_mutex_lock(&networkPacketsMutex);
        packet pkt = packets->back();
        cubeTail p;
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
                p.color[0] = 0.5;
                p.color[1] = 0.0;
                p.color[2] = 0.5;
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
                p.color[2] = 1.0;
                break;
            case 4:
                p.color[0] = 1.0;
                p.color[1] = 0.0;
                p.color[2] = 0.0;
                break;
        }
        packets->pop_back();
        pthread_mutex_unlock(&networkPacketsMutex);
        if (p.speed > 0) {
            p.cube = Cube(particleWidth / 2.0, p.curr, p.color[0], p.color[1], p.color[2]);
            p.tail = Tail(p.curr, p.start, p.color[0], p.color[1], p.color[2]);
            p.cube.generateBuffers();
            p.tail.generateBuffers();
            cubes.push_back(p);
        }
    }
    for (std::list<cubeTail>::iterator i = cubes.begin(); i != cubes.end(); i++) {
        cubeTail p = *i;
        bool keep = true;
        if (p.curr.x == p.dest.x && p.curr.y == p.dest.y) {
            p.cyclesToKeepAround--;
            if (p.cyclesToKeepAround <= 0) {
                keep = false;
                i = cubes.erase(i);
            }
        }
        if (keep) {
            if (!is3d)
                p.curr = calculateCurrentPosition(p.curr, p.dest, p.speed);
            else
                p.curr = calculateCurrentPosition3d(p.curr, p.dest, p.start, p.speed);
            i = cubes.erase(i);
            i = cubes.insert(i, p);
        }
    }
    cubes.sort(sortCubeTail);

    for (std::list<cubeTail>::iterator i = cubes.begin(); i != cubes.end(); i++) {
        cubeTail ct = *i;
        Cube c = ct.cube;
        Tail t = ct.tail;

        double fade = (double)ct.cyclesToKeepAround / (double)totalCycles;
        c.center = ct.curr;
        c.render3(fade);
        t.center = ct.curr;
        t.render3(fade);
    }
}

graphics::~graphics()
{
    glUseProgram(0);
    glDetachShader(shaderprogram, vrtShader);
    glDetachShader(shaderprogram, frgShader);
    glDeleteProgram(shaderprogram);
    glDeleteShader(vrtShader);
    glDeleteShader(frgShader);
    glDeleteVertexArrays(1, &vertexArrayObject);
}
