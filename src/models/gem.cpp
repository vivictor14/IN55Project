#include <QtGui/QOpenGLVertexArrayObject>
#include <QtGui/QOpenGLBuffer>
#include <src/util/vertex.h>
#include "gem.h"

Gem::Gem(GLfloat topHeight, GLfloat bottomHeight, GLfloat topRadius, GLfloat middleRadius, GLfloat bottomRadius, GLint topNbPoints, GLint middleNbPoints,
         GLint bottomNbPoints, GLint topComplexity, GLint bottomComplexity, QVector3D color) {

    this->topHeight = topHeight;
    this->bottomHeight = bottomHeight;
    this->topRadius = topRadius;
    this->middleRadius = middleRadius;
    this->bottomRadius = bottomRadius;
    this->topNbPoints = topNbPoints;
    this->middleNbPoints = middleNbPoints;
    this->bottomNbPoints = bottomNbPoints;
    this->topComplexity = topComplexity;
    this->bottomComplexity = bottomComplexity;

    initVertices(color);
    initMapping();
}

Gem::~Gem() {
    for(int i = 0; i < 4; i++) {
        vbo[i].destroy();
        vao[i].destroy();
    }
}

void Gem::initVertices(QVector3D color) {
    middleVertices = new Vertex [ middleNbPoints ];

    double angle = 2.0f * M_PI / middleNbPoints;
    for(int i = 0; i < middleNbPoints; i++) {
        float x = (float) cos(i * angle) * middleRadius;
        float z = (float) sin(i * angle) * middleRadius;
        middleVertices[i] = Vertex( QVector3D(x, 0, z), color);
    }
    calculateInnerMiddleRadius();

    topVertices = initFaceVertices(topHeight, topRadius, topNbPoints, topComplexity, color);
    bottomVertices = initFaceVertices(bottomHeight, bottomRadius, bottomNbPoints, bottomComplexity, color);

}

void Gem::calculateInnerMiddleRadius() {

    QVector3D A = middleVertices[0].position();
    QVector3D B = middleVertices[1].position();
    QVector3D M = QVector3D((A.x() + B.x()) / 2, (A.y() + B.y()) / 2, (A.z() + B.z()) / 2);
    innerMiddleRadius = (GLfloat) sqrt(pow(M.x(), 2) + pow(M.y(), 2) + pow(M.z(), 2));

}

Vertex *Gem::initFaceVertices(GLfloat height, GLfloat radius, GLint nbPoints, GLint complexity, QVector3D color) {

    Vertex *vertices;

    if(nbPoints > 1) {

        vertices = new Vertex[ complexity * nbPoints ];
        double angle = 2.0f * M_PI / nbPoints;
        double offset;

        for (int i = 0; i < complexity; i++) {

            offset = (i + 1) * angle / 2;

            for (int j = 0; j < nbPoints; j++) {

                float x, y, z;
                float complexityCircleRadius = radius + (complexity - 1 - i) * ((innerMiddleRadius - radius) / complexity);
                x = (float) cos(i * angle + offset) * complexityCircleRadius;
                z = (float) sin(i * angle + offset) * complexityCircleRadius;
                y = i * (height / complexity);
                vertices[(i * nbPoints) + j] = Vertex(QVector3D(x, y, z), color);

            }
        }
    }

    else {

        vertices = new Vertex[ (complexity - 1) * middleNbPoints + 1 ];
        vertices[0] = Vertex(QVector3D(0, height, 0));

        double angle = 2.0f * M_PI / middleNbPoints;
        double offset;

        for (int i = 0; i < complexity - 1; i++) {

            if ((complexity - i) % 2 != 0) {
                offset = angle / 2;
            } else {
                offset = 0;
            }

            for (int j = 0; j < middleNbPoints; j++) {

                float x, y, z;
                float complexityCircleRadius = innerMiddleRadius - i * (innerMiddleRadius / complexity);
                x = (float) cos(i * angle + offset) * complexityCircleRadius;
                z = (float) sin(i * angle + offset) * complexityCircleRadius;
                y = i * (height / complexity);
                vertices[(i * nbPoints) + j] = Vertex(QVector3D(x, y, z), color);

            }

        }

    }

    return vertices;

}

void Gem::initMapping() {

    drawVertices = new Vertex* [6];

    drawVertices[0] = initFanFaceMapping(bottomVertices, bottomNbPoints, bottomComplexity);
    drawVertices[1] = initStripFaceMapping(bottomVertices, bottomNbPoints, bottomComplexity);
    drawVertices[2] = initTrianglesFaceMapping(bottomVertices, bottomNbPoints, bottomComplexity);

    drawVertices[3] = initFanFaceMapping(topVertices, topNbPoints, topComplexity);
    drawVertices[4] = initStripFaceMapping(topVertices, topNbPoints, topComplexity);
    drawVertices[5] = initTrianglesFaceMapping(topVertices, topNbPoints, topComplexity);

}

Vertex *Gem::initFanFaceMapping(Vertex *vertices, GLint nbPoints, GLint complexity) {

    Vertex *fanVertices;

    if(nbPoints > 1) {
        fanVertices = new Vertex[nbPoints];

        for(int i = 0; i < nbPoints; i++) {

            fanVertices[i] = vertices[i];

        }
    }
    else {
        int length = (complexity - 1) * middleNbPoints + 1;

        fanVertices = new Vertex[middleNbPoints + 2];
        fanVertices[0] = vertices[0];
        for(int i = 0; i < middleNbPoints; i++) {
            fanVertices[i + 1] = vertices[length - middleNbPoints + i];
        }
        fanVertices[middleNbPoints + 1] = vertices[length - middleNbPoints];
    }

    return fanVertices;

}

Vertex *Gem::initStripFaceMapping(Vertex *vertices, GLint nbPoints, GLint complexity) {

    Vertex *stripVertices;

    if(nbPoints > 1) {

        stripVertices = new Vertex[(complexity - 1) * 2 * nbPoints + 2];

        for(int i = 0; i < (complexity - 1) * nbPoints; i++) {

            stripVertices[2 * i] = vertices[i];
            stripVertices[2 * i + 1] = vertices[nbPoints + i];

        }
        stripVertices[(complexity - 1) * 2 * nbPoints] = vertices[0];
        stripVertices[(complexity - 1) * 2 * nbPoints + 1] = vertices[nbPoints];
    }
    else {

    }

    return stripVertices;

}

Vertex *Gem::initTrianglesFaceMapping(Vertex *vertices, GLint nbPoints, GLint complexity) {

    Vertex *trianglesVertices;

    if(nbPoints > 1) {
        trianglesVertices = new Vertex[3 * nbPoints + 3 * middleNbPoints];

        for(int i = 0; i < nbPoints; i++) {

            trianglesVertices[3 * i] = vertices[(complexity - 1) * nbPoints + i];
            trianglesVertices[3 * i + 1] = vertices[(complexity - 1) * nbPoints + i + 1];
            trianglesVertices[3 * i + 2] = middleVertices[i * middleNbPoints / nbPoints];

        }

        for(int i = 0; i < middleNbPoints; i++) {

            trianglesVertices[3 * nbPoints + 3 * i] = middleVertices[i];
            trianglesVertices[3 * nbPoints + 3 * i + 1] = middleVertices[i + 1];
            trianglesVertices[3 * nbPoints + 3 * i + 2] = vertices[(complexity - 1) * nbPoints + i * nbPoints / middleNbPoints];

        }
    }

    return trianglesVertices;

}

void Gem::initializeBuffer(QOpenGLShaderProgram *shaderProgram) {

    initializeOpenGLFunctions();

    vbo = new QOpenGLBuffer[6];
    vao = new QOpenGLVertexArrayObject[6];

    for(int i = 0; i < 6; i++) {
        vbo[i].create();
        vbo[i].bind();
        vbo[i].setUsagePattern(QOpenGLBuffer::StaticDraw);
        vbo[i].allocate(drawVertices[i], sizeof(drawVertices[i]));

        vao[i].create();
        vao[i].bind();
        shaderProgram->enableAttributeArray(0);
        shaderProgram->enableAttributeArray(1);
        shaderProgram->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
        shaderProgram->setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());

        vao[i].release();
        vbo[i].release();
    }

}

void Gem::drawShape(QOpenGLShaderProgram *shaderProgram, int u_modelToWorld, Transform3D m_transform) {

    for(int i = 0; i < 6; i++) {
        vao[i].bind();
        shaderProgram->setUniformValue(u_modelToWorld, m_transform.toMatrix());
        if(i%3 == 0) {
            glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(drawVertices[i]) / sizeof(drawVertices[i][0]));
        }
        else if(i%3 == 1){
            glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(drawVertices[i]) / sizeof(drawVertices[i][0]));
        }
        else {
            glDrawArrays(GL_TRIANGLES, 0, sizeof(drawVertices[i]) / sizeof(drawVertices[i][0]));
        }
        vao[i].release();
    }

}
