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

    topVertices = initFaceVertices(true, topHeight, topRadius, topNbPoints, topComplexity, color);
    bottomVertices = initFaceVertices(false, bottomHeight, bottomRadius, bottomNbPoints, bottomComplexity, color);

}

void Gem::calculateInnerMiddleRadius() {

    QVector3D A = middleVertices[0].position();
    QVector3D B = middleVertices[1].position();
    QVector3D M = QVector3D((A.x() + B.x()) / 2, (A.y() + B.y()) / 2, (A.z() + B.z()) / 2);
    innerMiddleRadius = (GLfloat) sqrt(pow(M.x(), 2) + pow(M.y(), 2) + pow(M.z(), 2));

}

Vertex *Gem::initFaceVertices(bool counterClockWise, GLfloat height, GLfloat radius, GLint nbPoints, GLint complexity, QVector3D color) {

    Vertex *vertices;

    if(nbPoints > 1) {

        vertices = new Vertex[ complexity * nbPoints ];
        double angle = 2.0f * M_PI / nbPoints;
        double offset;

        for (int i = 0; i < complexity; i++) {

            offset = (i + 1) * angle / 2;

            for (int j = 0; j < nbPoints; j++) {

                float x, y, z;
                float complexityCircleRadius = radius + (complexity - 1 - i) * ((middleRadius - radius) / complexity);
                x = (float) cos(j * angle + offset) * complexityCircleRadius;
                z = (float) sin((j * angle + offset) * (2 * counterClockWise - 1)) * complexityCircleRadius;
                y = (i + 1) * (height / complexity);
                vertices[(i * nbPoints) + j] = Vertex(QVector3D(x, y, z), color);

            }
        }
    }

    else {

        vertices = new Vertex[ (complexity - 1) * middleNbPoints + 1 ];
        vertices[0] = Vertex(QVector3D(0, height, 0), color);

        double angle = 2.0f * M_PI / middleNbPoints;
        double offset;

        for (int i = 0; i < complexity - 1; i++) {

            offset = (i + 1) * angle / 2;

            for (int j = 0; j < middleNbPoints; j++) {

                float x, y, z;
                float complexityCircleRadius = middleRadius - (i + 1) * (middleRadius / complexity);
                x = (float) cos(j * angle + offset) * complexityCircleRadius;
                z = (float) sin((j * angle + offset) * (2 * counterClockWise - 1)) * complexityCircleRadius;
                y = (i + 1) * (height / complexity);
                vertices[(i * middleNbPoints) + j + 1] = Vertex(QVector3D(x, y, z), color);

            }

        }

    }

    return vertices;

}

void Gem::initMapping() {

    drawVertices = new Vertex*[6];
    length = new int[6];

    drawVertices[0] = initFanFaceMapping(&length[0], bottomVertices, bottomNbPoints, bottomComplexity, true);
    drawVertices[1] = initStripFaceMapping(&length[1], bottomVertices, bottomNbPoints, bottomComplexity, true);
    drawVertices[2] = initTrianglesFaceMapping(&length[2], bottomVertices, bottomNbPoints, true);

    drawVertices[3] = initFanFaceMapping(&length[3], topVertices, topNbPoints, topComplexity, false);
    drawVertices[4] = initStripFaceMapping(&length[4], topVertices, topNbPoints, topComplexity, false);
    drawVertices[5] = initTrianglesFaceMapping(&length[5], topVertices, topNbPoints, false);

}

Vertex *Gem::initFanFaceMapping(int *length, Vertex *vertices, GLint nbPoints, GLint complexity, bool clockWise) {

    Vertex *fanVertices;

    if(nbPoints > 1) {
        *length = nbPoints;
        fanVertices = new Vertex[nbPoints];

        for(int i = 0; i < nbPoints; i++) {

            fanVertices[i] = vertices[(complexity * nbPoints - 1) - i];

        }
    }
    else {
        *length = middleNbPoints + 2;

        fanVertices = new Vertex[middleNbPoints + 2];
        fanVertices[0] = vertices[0];
        if(complexity > 1) {
            for (int i = 0; i <= middleNbPoints; i++) {

                fanVertices[i + 1] = vertices[(complexity - 1) * middleNbPoints - i % middleNbPoints];

            }
        }
        else {
            for (int i = 0; i <= middleNbPoints; i++) {
                if(clockWise) {
                    fanVertices[i + 1] = middleVertices[(middleNbPoints - 1) - i % middleNbPoints];
                }
                else {
                    fanVertices[i + 1] = middleVertices[i % middleNbPoints];
                }
            }
        }
    }

    return fanVertices;

}

Vertex *Gem::initStripFaceMapping(int *length, Vertex *vertices, GLint nbPoints, GLint complexity, bool clockWise) {

    Vertex *stripVertices = new Vertex[0];
    *length = 0;

    if(complexity > 1) {
        if (nbPoints > 1) {

            *length = (complexity - 1) * 2 * (nbPoints + 1);
            stripVertices = new Vertex[*length];

            for(int i = 0; i < complexity - 1; i++) {

                for(int j = 0; j <= nbPoints; j++) {

                    stripVertices[2 * (i * (nbPoints + 1) + j)] = vertices[i * nbPoints + j % nbPoints];
                    stripVertices[2 * (i * (nbPoints + 1) + j) + 1] = vertices[(i + 1) * nbPoints + j % nbPoints];

                }

            }

        }

        else {

            *length = (complexity - 1) * 2 * (middleNbPoints + 1);
            stripVertices = new Vertex[*length];

            for(int i = 0; i <= middleNbPoints; i++) {

                if(clockWise) {
                    stripVertices[2 * i] = middleVertices[(middleNbPoints - i) % middleNbPoints];
                    stripVertices[2 * i + 1] = vertices[i % middleNbPoints + 1];
                }
                else {
                    stripVertices[2 * i] = middleVertices[i % middleNbPoints];
                    stripVertices[2 * i + 1] = vertices[i % middleNbPoints + 1];
                }

            }

            for(int i = 1; i < complexity - 1; i++) {
                for(int j = 0; j <= middleNbPoints; j++) {

                    stripVertices[2 * (i * (middleNbPoints + 1) + j)] = vertices[(i - 1) * middleNbPoints + 1 + j % middleNbPoints];
                    stripVertices[2 * (i * (middleNbPoints + 1) + j) + 1] = vertices[i * middleNbPoints + 1 + j % middleNbPoints];

                }

            }

        }
    }

    return stripVertices;

}

Vertex *Gem::initTrianglesFaceMapping(int *length, Vertex *vertices, GLint nbPoints, bool clockwise) {

    Vertex *trianglesVertices = new Vertex[0];

    if(nbPoints > 1) {

        *length = 3 * nbPoints + 3 * middleNbPoints;
        trianglesVertices = new Vertex[*length];

        for(int i = 0; i < nbPoints; i++) {

            if(clockwise){
                trianglesVertices[3 * i + 1] = vertices[nbPoints - 1 - i];
                trianglesVertices[3 * i] = vertices[(nbPoints - 1) - (i+1)%nbPoints];
                trianglesVertices[3 * i + 2] = middleVertices[(i + 1) % nbPoints * middleNbPoints / nbPoints];
            }
            else {
                trianglesVertices[3 * i] = vertices[i];
                trianglesVertices[3 * i + 1] = vertices[(i + 1) % nbPoints];
                trianglesVertices[3 * i + 2] = middleVertices[(i + 1) % nbPoints * middleNbPoints / nbPoints];
            }

        }

        for(int i = 0; i < middleNbPoints; i++) {

            if(clockwise){
                trianglesVertices[3 * nbPoints + 3 * i] = middleVertices[i];
                trianglesVertices[3 * nbPoints + 3 * i + 1] = middleVertices[(i + 1) % middleNbPoints];
                trianglesVertices[3 * nbPoints + 3 * i + 2] = vertices[nbPoints - 1 - (i * nbPoints / middleNbPoints)];
            }
            else {
                trianglesVertices[3 * nbPoints + 3 * i + 1] = middleVertices[i];
                trianglesVertices[3 * nbPoints + 3 * i] = middleVertices[(i + 1) % middleNbPoints];
                trianglesVertices[3 * nbPoints + 3 * i + 2] = vertices[i * nbPoints / middleNbPoints];
            }

        }
    }

    return trianglesVertices;

}

void Gem::initializeBuffer(QOpenGLShaderProgram *shaderProgram) {

    initializeOpenGLFunctions();

    vbo = new QOpenGLBuffer[6];
    vao = new QOpenGLVertexArrayObject[6];

//    vbo[0].create();
//    vbo[0].bind();
//    vbo[0].setUsagePattern(QOpenGLBuffer::StaticDraw);
//    vbo[0].allocate(topVertices, topComplexity * topNbPoints * sizeof(Vertex));
//
//    vao[0].create();
//    vao[0].bind();
//    shaderProgram->enableAttributeArray(0);
//    shaderProgram->enableAttributeArray(1);
//    shaderProgram->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
//    shaderProgram->setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());
//
//    vao[0].release();
//    vbo[0].release();
//
//    vbo[1].create();
//    vbo[1].bind();
//    vbo[1].setUsagePattern(QOpenGLBuffer::StaticDraw);
//    vbo[1].allocate(bottomVertices, ((bottomComplexity - 1) * middleNbPoints + 1) * sizeof(Vertex));
//
//    vao[1].create();
//    vao[1].bind();
//    shaderProgram->enableAttributeArray(0);
//    shaderProgram->enableAttributeArray(1);
//    shaderProgram->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
//    shaderProgram->setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());
//
//    vao[1].release();
//    vbo[1].release();
//
//    vbo[2].create();
//    vbo[2].bind();
//    vbo[2].setUsagePattern(QOpenGLBuffer::StaticDraw);
//    vbo[2].allocate(middleVertices, middleNbPoints * sizeof(Vertex));
//
//    vao[2].create();
//    vao[2].bind();
//    shaderProgram->enableAttributeArray(0);
//    shaderProgram->enableAttributeArray(1);
//    shaderProgram->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
//    shaderProgram->setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());
//
//    vao[2].release();
//    vbo[2].release();


    for(int i = 0; i < 6; i++) {
        vbo[i].create();
        vbo[i].bind();
        vbo[i].setUsagePattern(QOpenGLBuffer::StaticDraw);
        vbo[i].allocate(drawVertices[i], length[i] * sizeof(Vertex));

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
            glDrawArrays(GL_TRIANGLE_FAN, 0, length[i]);
        }
        else if(i%3 == 1){
            glDrawArrays(GL_TRIANGLE_STRIP, 0, length[i]);
        }
        else {
            glDrawArrays(GL_TRIANGLES, 0, length[i]);
        }
        vao[i].release();
    }

//    vao[0].bind();
//    shaderProgram->setUniformValue(u_modelToWorld, m_transform.toMatrix());
//    glDrawArrays(GL_POINTS, 0, topComplexity * topNbPoints);
//    vao[0].release();
//    vao[1].bind();
//    shaderProgram->setUniformValue(u_modelToWorld, m_transform.toMatrix());
//    glDrawArrays(GL_POINTS, 0, (bottomComplexity - 1) * middleNbPoints + 1);
//    vao[1].release();
//    vao[2].bind();
//    shaderProgram->setUniformValue(u_modelToWorld, m_transform.toMatrix());
//    glDrawArrays(GL_POINTS, 0, middleNbPoints);
//    vao[2].release();

}
