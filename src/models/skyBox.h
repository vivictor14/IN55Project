//
// Created by Anthony on 09/06/2017.
//

#ifndef IN55PROJECT_CUBEMAP_H
#define IN55PROJECT_CUBEMAP_H

#include <GL/gl.h>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QVector3D>
#include <QtGui/QOpenGLShaderProgram>
#include <src/util/transform3D.h>
#include <QtGui/QOpenGLVertexArrayObject>
#include <QtGui/QOpenGLBuffer>
#include <src/util/vertex.h>
#include <vector>
#include <QtGui/QColor>
#include "gem.h"

class skyBox : protected QOpenGLFunctions {
private:
    QOpenGLBuffer *vbo;
    QOpenGLVertexArrayObject *vao;
    VerticesMapping *mapping;
    unsigned int cubemapTexture;

    unsigned int loadCubemap(std::vector<std::string> faces);

public:
    skyBox();

    ~skyBox();

    void initializeBuffer(QOpenGLShaderProgram *shaderProgram);

    void drawShape();

    unsigned int getTexture();


};
#endif //IN55PROJECT_CUBEMAP_H
