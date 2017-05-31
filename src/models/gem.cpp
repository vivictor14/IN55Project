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
    mapping();
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
    QVector3D M = QVector3D((A.x() + B.x()) / 2,
                            (A.y() + B.y()) / 2,
                            (A.z() + B.z()) / 2);
    innerMiddleRadius = (GLfloat) sqrt(pow(M.x(), 2) + pow(M.y(), 2) + pow(M.z(), 2));

}

float Gem::calculateComplexityCircleRadius(GLfloat height, GLfloat radius, GLfloat middleRadius, GLint nbPoints, GLint complexity, int index) {

    float k = index / (float)(complexity);
    QVector3D A = QVector3D(radius, height, 0);
    QVector3D B = QVector3D((float) (middleRadius * cos(2 * M_PI / nbPoints)), 0,
                            (float) (middleRadius * sin(2 * M_PI / nbPoints)));
    QVector3D u = A - B;
    QVector3D M = QVector3D(k * u.x() + B.x(), k * u.y() + B.y(), k * u.z() + B.z());

    QVector3D test = QVector3D(0, k * height, 0);
    float rep = M.distanceToPoint(test);

    return M.distanceToPoint(QVector3D(0, k * height, 0));

}

Vertex *Gem::initFaceVertices(bool counterClockWise, GLfloat height, GLfloat radius, GLint nbPoints, GLint complexity, QVector3D color) {

    Vertex *vertices;

    if(nbPoints > 1) {

        vertices = new Vertex[ complexity * nbPoints ];
        double angle = 2.0f * M_PI / nbPoints;
        double offset;

        for (int i = 0; i < complexity; i++) {

            offset = (i + 1) * angle / 2;
            float complexityCircleRadius;
            if(i < complexity - 1) {
                complexityCircleRadius = calculateComplexityCircleRadius(height, radius, innerMiddleRadius, nbPoints, complexity, i + 1);
            }
            else {
                complexityCircleRadius = radius;
            }

            for (int j = 0; j < nbPoints; j++) {

                float x, y, z;

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
            float complexityCircleRadius = calculateComplexityCircleRadius(height, 0, innerMiddleRadius, middleNbPoints, complexity, i + 1);

            for (int j = 0; j < middleNbPoints; j++) {

                float x, y, z;

                x = (float) cos(j * angle + offset) * complexityCircleRadius;
                z = (float) sin((j * angle + offset) * (2 * counterClockWise - 1)) * complexityCircleRadius;
                y = (i + 1) * (height / complexity);
                vertices[(i * middleNbPoints) + j + 1] = Vertex(QVector3D(x, y, z), color);

            }

        }

    }

    return vertices;

}

void Gem::mapping() {

    drawVertices = new Vertex*[8];
    length = new int[8];

    if(bottomNbPoints > 1) {
        drawVertices[0] = tableMapping(&length[0], bottomVertices, bottomNbPoints, bottomComplexity, true);
    }
    else {
        drawVertices[0] = pavilionMapping(&length[0], bottomVertices, bottomNbPoints, bottomComplexity, true);
    }
    drawVertices[1] = starMapping(&length[1], bottomVertices, bottomNbPoints, bottomComplexity, true);
    drawVertices[2] = bezelMapping(&length[2], bottomVertices, bottomNbPoints, bottomComplexity, true);
    drawVertices[3] = upperGirdleMapping(&length[3], bottomVertices, bottomNbPoints, true);

    if(topNbPoints > 1) {
        drawVertices[4] = tableMapping(&length[4], topVertices, topNbPoints, topComplexity, false);
    }
    else {
        drawVertices[4] = pavilionMapping(&length[4], topVertices, topNbPoints, topComplexity, false);
    }
    drawVertices[5] = starMapping(&length[5], topVertices, topNbPoints, topComplexity, false);
    drawVertices[6] = bezelMapping(&length[6], topVertices, topNbPoints, topComplexity, false);
    drawVertices[7] = upperGirdleMapping(&length[7], topVertices, topNbPoints, false);

}

Vertex *Gem::tableMapping(int *length, Vertex *vertices, GLint nbPoints, GLint complexity, bool clockWise) {

    Vertex *tableVertices;

    *length = nbPoints;
    tableVertices = new Vertex[nbPoints];

    for(int i = 0; i < nbPoints; i++) {

        tableVertices[i] = vertices[(complexity * nbPoints - 1) - i];

    }

    return tableVertices;

}

Vertex *Gem::pavilionMapping(int *length, Vertex *vertices, GLint nbPoints, GLint complexity, bool clockWise) {

    Vertex *pavilionVertices;

    *length = middleNbPoints + 2;

    pavilionVertices = new Vertex[middleNbPoints + 2];
    pavilionVertices[0] = vertices[0];
    if(complexity > 1) {
        for (int i = 0; i <= middleNbPoints; i++) {

            pavilionVertices[i + 1] = vertices[(complexity - 1) * middleNbPoints - i % middleNbPoints];

        }
    }
    else {
        for (int i = 0; i <= middleNbPoints; i++) {
            if(clockWise) {
                pavilionVertices[i + 1] = middleVertices[(middleNbPoints - 1) - i % middleNbPoints];
            }
            else {
                pavilionVertices[i + 1] = middleVertices[i % middleNbPoints];
            }
        }
    }

    return pavilionVertices;

}

Vertex *Gem::starMapping(int *length, Vertex *vertices, GLint nbPoints, GLint complexity, bool clockWise) {

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

Vertex *Gem::bezelMapping(int *length, Vertex *vertices, GLint nbPoints, GLint complexity, bool clockWise) {
    return nullptr;
}

Vertex *Gem::upperGirdleMapping(int *length, Vertex *vertices, GLint nbPoints, bool clockwise) {

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

    vbo = new QOpenGLBuffer[8];
    vao = new QOpenGLVertexArrayObject[8];

    vbo[0].create();
    vbo[0].bind();
    vbo[0].setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo[0].allocate(topVertices, topComplexity * topNbPoints * sizeof(Vertex));

    vao[0].create();
    vao[0].bind();
    shaderProgram->enableAttributeArray(0);
    shaderProgram->enableAttributeArray(1);
    shaderProgram->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
    shaderProgram->setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());

    vao[0].release();
    vbo[0].release();

    vbo[1].create();
    vbo[1].bind();
    vbo[1].setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo[1].allocate(bottomVertices, ((bottomComplexity - 1) * middleNbPoints + 1) * sizeof(Vertex));

    vao[1].create();
    vao[1].bind();
    shaderProgram->enableAttributeArray(0);
    shaderProgram->enableAttributeArray(1);
    shaderProgram->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
    shaderProgram->setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());

    vao[1].release();
    vbo[1].release();

    vbo[2].create();
    vbo[2].bind();
    vbo[2].setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo[2].allocate(middleVertices, middleNbPoints * sizeof(Vertex));

    vao[2].create();
    vao[2].bind();
    shaderProgram->enableAttributeArray(0);
    shaderProgram->enableAttributeArray(1);
    shaderProgram->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
    shaderProgram->setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());

    vao[2].release();
    vbo[2].release();


//    for(int i = 0; i < 8; i++) {
//        vbo[i].create();
//        vbo[i].bind();
//        vbo[i].setUsagePattern(QOpenGLBuffer::StaticDraw);
//        vbo[i].allocate(drawVertices[i], length[i] * sizeof(Vertex));
//
//        vao[i].create();
//        vao[i].bind();
//        shaderProgram->enableAttributeArray(0);
//        shaderProgram->enableAttributeArray(1);
//        shaderProgram->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
//        shaderProgram->setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());
//
//        vao[i].release();
//        vbo[i].release();
//    }

}

void Gem::drawShape(QOpenGLShaderProgram *shaderProgram, int u_modelToWorld, Transform3D m_transform) {

//    for(int i = 0; i < 8; i++) {
//        vao[i].bind();
//        shaderProgram->setUniformValue(u_modelToWorld, m_transform.toMatrix());
//        if(i%3 == 0) {
//            glDrawArrays(GL_TRIANGLE_FAN, 0, length[i]);
//        }
//        else if(i%3 == 1){
//            glDrawArrays(GL_TRIANGLE_STRIP, 0, length[i]);
//        }
//        else {
//            glDrawArrays(GL_TRIANGLES, 0, length[i]);
//        }
//        vao[i].release();
//    }

    vao[0].bind();
    shaderProgram->setUniformValue(u_modelToWorld, m_transform.toMatrix());
    glDrawArrays(GL_POINTS, 0, topComplexity * topNbPoints);
    vao[0].release();
    vao[1].bind();
    shaderProgram->setUniformValue(u_modelToWorld, m_transform.toMatrix());
    glDrawArrays(GL_POINTS, 0, (bottomComplexity - 1) * middleNbPoints + 1);
    vao[1].release();
    vao[2].bind();
    shaderProgram->setUniformValue(u_modelToWorld, m_transform.toMatrix());
    glDrawArrays(GL_POINTS, 0, middleNbPoints);
    vao[2].release();

}
