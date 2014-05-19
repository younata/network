#include "glObject.h"
#include <stdio.h>

GLObject::GLObject()
{
    isGL3 = false;
    verts = NULL;
    colors = NULL;
}

void GLObject::generateBuffers()
{
    glGenVertexArrays(1, &vertexArrayObject);
    glGenBuffers(2, vertexBufferObject);
    isGL3 = true;
}

void GLObject::render()
{
    if (!isGL3)
        return;
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);


    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
}

GLObject::~GLObject()
{
    if (isGL3) {
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDeleteBuffers(2, vertexBufferObject);
    }
}
