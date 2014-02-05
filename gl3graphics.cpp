#include "graphics.h"

#include <assert.h>
#include <unistd.h>

#include <algorithm>

#include <time.h>
#include <list>

#include "cube.h"
#include "tail.h"

extern pthread_mutex_t networkPacketsMutex;

struct particle {
    struct point3d start;
    struct point3d dest;
    struct point3d curr;

    double speed;

    int cyclesToKeepAround;
    
    double color[3];
};

std::list<packet> *packets;

std::list<particle> *particles;

int totalCycles = 30;

double maxWidth = 10.0;
double particleWidth = 0.025;

struct point3d windowPos;
struct point3d windowSize;
bool isFullScreen = false;

bool is3d = false;

struct point3d curMousePos;
char mouseAddr[18];

class graphics {
private:
    GLuint vtxShader, frgShader;
    GLuint shaderprogram;

    GLuint vertexArrayObject;
    GLuint vertexBufferObject[2];

    std::list<cube> cubes;

public:
    graphics();

    void generateBuffers();
    ~graphics();
};

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
