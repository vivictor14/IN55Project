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

float Gem::calculateComplexityCircleRadius(GLfloat height, GLfloat radius, GLint nbPoints, GLint complexity, int index) {

    float k = index / (float)(complexity);
//    QVector3D A = QVector3D(radius, height, 0);
//    QVector3D B = QVector3D((float) (middleRadius * cos(2 * M_PI / nbPoints)), 0,
//                            (float) (middleRadius * sin(2 * M_PI / nbPoints)));
//    QVector3D u = A - B;
//    QVector3D M = QVector3D(k * u.x() + B.x(), k * u.y() + B.y(), k * u.z() + B.z());
//
//    QVector3D test = QVector3D(0, k * height, 0);
//    float rep = M.distanceToPoint(test);
//
//    return M.distanceToPoint(QVector3D(0, k * height, 0));
    return innerMiddleRadius - (innerMiddleRadius - radius) * k;

}

Vertex *Gem::initFaceVertices(bool counterClockWise, GLfloat height, GLfloat radius, GLint nbPoints, GLint complexity, QVector3D color) {

    Vertex *vertices;

    int culet = 0;
    if(nbPoints == 1) {
        radius = 0;
        nbPoints = middleNbPoints;
        culet = 1;
    }

    vertices = new Vertex[ (complexity - culet) * nbPoints + culet ];
    double angle = 2.0f * M_PI / nbPoints;
    double offset;

    for (int i = 0; i < complexity - culet; i++) {

        offset = (i + 1) * angle / 2;
        float complexityCircleRadius = calculateComplexityCircleRadius(height, radius, nbPoints, complexity, i + 1);


        for (int j = 0; j < nbPoints; j++) {

            float x, y, z;

            x = (float) cos(j * angle + offset) * complexityCircleRadius;
            z = (float) sin((j * angle + offset) * (2 * counterClockWise - 1)) * complexityCircleRadius;
            y = (i + 1) * (height / complexity);
            vertices[(i * nbPoints) + j] = Vertex(QVector3D(x, y, z), color);

        }
    }

    if(culet) {
        vertices[(complexity - culet) * nbPoints] = Vertex(QVector3D(0, height, 0), color);
    }

    return vertices;

}

void Gem::mapping() {

    mappings = new VerticesMapping[8];

    if(bottomNbPoints > 1) {
        tableMapping(&mappings[0], bottomVertices, bottomNbPoints, bottomComplexity);
        if(bottomComplexity > 1) {
            starMapping(&mappings[1], bottomVertices, bottomNbPoints, bottomComplexity);
            if(bottomComplexity > 2) {
                bezelMapping(&mappings[2], bottomVertices, bottomNbPoints, bottomComplexity);
            }
        }
        upperGirdleMapping(&mappings[3], bottomVertices, bottomNbPoints, bottomComplexity, true);
    }
    else {
        pavilionMapping(&mappings[0], bottomVertices, bottomComplexity, true);
        if(bottomComplexity > 1) {
            if (bottomComplexity > 2) {
                starMapping(&mappings[1], bottomVertices, middleNbPoints, bottomComplexity - 1);
                bezelMapping(&mappings[2], bottomVertices, middleNbPoints, bottomComplexity - 1);
            }
            upperGirdleMapping(&mappings[3], bottomVertices, middleNbPoints, bottomComplexity - 1, true);
        }
    }

    if(topNbPoints > 1) {
        tableMapping(&mappings[4], topVertices, topNbPoints, topComplexity);
        if(topComplexity > 1) {
            starMapping(&mappings[5], topVertices, topNbPoints, topComplexity);

            if(topComplexity > 2) {
                bezelMapping(&mappings[6], topVertices, topNbPoints, topComplexity);
            }
        }
        upperGirdleMapping(&mappings[7], topVertices, topNbPoints, topComplexity, false);
    }
    else {
        pavilionMapping(&mappings[4], topVertices, topComplexity, false);
        if(topComplexity > 1) {
            if (topComplexity > 2) {
                starMapping(&mappings[5], topVertices, middleNbPoints, topComplexity - 1);
                bezelMapping(&mappings[6], topVertices, middleNbPoints, topComplexity - 1);
            }
            upperGirdleMapping(&mappings[7], topVertices, middleNbPoints, topComplexity - 1, false);
        }
    }

}

void Gem::tableMapping(VerticesMapping *mapping, Vertex *vertices, GLint nbPoints, GLint complexity) {
    
    mapping->mode = GL_TRIANGLE_FAN;
    mapping->length = nbPoints;
    mapping->vertices = new Vertex[nbPoints];

    for(int i = 0; i < nbPoints; i++) {

        mapping->vertices[i] = vertices[(complexity * nbPoints - 1) - i];

    }

}

void Gem::pavilionMapping(VerticesMapping *mapping, Vertex *vertices, GLint complexity, bool clockWise) {

    mapping->mode = GL_TRIANGLE_FAN;
    mapping->length  = middleNbPoints + 2;
    mapping->vertices = new Vertex[middleNbPoints + 2];
    mapping->vertices[0] = vertices[middleNbPoints * (complexity - 1)];
    if(complexity > 1) {
        for (int i = 0; i <= middleNbPoints; i++) {

            mapping->vertices[i + 1] = vertices[(complexity - 1) * middleNbPoints - 1 - i % middleNbPoints];

        }
    }
    else {
        for (int i = 0; i <= middleNbPoints; i++) {
            if(clockWise) {
                mapping->vertices[i + 1] = middleVertices[i % middleNbPoints];
            }
            else {
                mapping->vertices[i + 1] = middleVertices[(middleNbPoints - 1) - i % middleNbPoints];
            }
        }
    }

}

void Gem::starMapping(VerticesMapping *mapping, Vertex *vertices, GLint nbPoints, GLint complexity) {

    mapping->mode = GL_TRIANGLES;
    mapping->length = 3 * nbPoints;
    mapping->vertices = new Vertex[mapping->length];

    for(int i = 0; i < nbPoints; i++) {
        mapping->vertices[3 * i] = vertices[nbPoints * (complexity - 1) + i];
        mapping->vertices[3 * i + 1] = vertices[nbPoints * (complexity - 1) + (i + 1) % nbPoints];
        mapping->vertices[3 * i + 2] = vertices[nbPoints * (complexity - 2) + (i + 1) % nbPoints];
    }

}

void Gem::bezelMapping(VerticesMapping *mapping, Vertex *vertices, GLint nbPoints, GLint complexity) {

    mapping->mode = GL_TRIANGLES;
    mapping->length = 6 * nbPoints * (complexity - 2);
    mapping->vertices = new Vertex[mapping->length];

    for(int i = 0; i < complexity - 2; i++) {
        for(int j = 0; j < nbPoints; j++) {
            mapping->vertices[6 * (nbPoints * i + j)] = vertices[i * nbPoints + j];
            mapping->vertices[6 * (nbPoints * i + j) + 1] = vertices[(i + 1) * nbPoints + (nbPoints - 1 + j) % nbPoints];
            mapping->vertices[6 * (nbPoints * i + j) + 2] = vertices[(i + 2) * nbPoints + (nbPoints - 1 + j) % nbPoints];
            mapping->vertices[6 * (nbPoints * i + j) + 3] = vertices[(i + 2) * nbPoints + (nbPoints - 1 + j) % nbPoints];
            mapping->vertices[6 * (nbPoints * i + j) + 4] = vertices[(i + 1) * nbPoints + j];
            mapping->vertices[6 * (nbPoints * i + j) + 5] = vertices[i * nbPoints + j];
        }
    }


}

void Gem::upperGirdleMapping(VerticesMapping *mapping, Vertex *vertices, GLint nbPoints, GLint complexity, bool clockWise) {

    mapping->mode = GL_TRIANGLES;
    mapping->length = (1 + (complexity > 1)) * 3 * nbPoints + 3 * middleNbPoints;
    mapping->vertices = new Vertex[mapping->length];

    for(int i = 0; i < middleNbPoints; i++) {

        if(clockWise){
            mapping->vertices[3 * i] = middleVertices[i];
            mapping->vertices[3 * i + 1] = middleVertices[(i + 1) % middleNbPoints];
            mapping->vertices[3 * i + 2] = vertices[nbPoints - 1 - (i * nbPoints / middleNbPoints)];
        }
        else {
            mapping->vertices[3 * i] = middleVertices[(i + 1) % middleNbPoints];
            mapping->vertices[3 * i + 1] = middleVertices[i];
            mapping->vertices[3 * i + 2] = vertices[i * nbPoints / middleNbPoints];
        }

    }

    if(complexity == 1) {
        for (int i = 0; i < nbPoints; i++) {

            if(clockWise) {
                mapping->vertices[3 * middleNbPoints + 3 * i] = vertices[i];
                mapping->vertices[3 * middleNbPoints + 3 * i + 1] = vertices[(i + 1) % nbPoints];
                mapping->vertices[3 * middleNbPoints + 3 * i + 2] = middleVertices[nbPoints - 1 - (i * middleNbPoints / nbPoints)];
            }
            else {
                mapping->vertices[3 * middleNbPoints + 3 * i] = vertices[(nbPoints + i - 1) % nbPoints ];
                mapping->vertices[3 * middleNbPoints + 3 * i + 1] = vertices[i];
                mapping->vertices[3 * middleNbPoints + 3 * i + 2] = middleVertices[i * middleNbPoints / nbPoints];
            }

        }
    }
    else {

        for (int i = 0; i < nbPoints; i++) {

            if(clockWise) {
                mapping->vertices[3 * middleNbPoints + 6 * i] = vertices[nbPoints + i];
                mapping->vertices[3 * middleNbPoints + 6 * i + 1] = vertices[(i + 1) % nbPoints];
                mapping->vertices[3 * middleNbPoints + 6 * i + 2] = middleVertices[nbPoints - 1 - (i * middleNbPoints / nbPoints)];
                mapping->vertices[3 * middleNbPoints + 6 * i + 3] = middleVertices[nbPoints - 1 - (i * middleNbPoints / nbPoints)];
                mapping->vertices[3 * middleNbPoints + 6 * i + 4] = vertices[i];
                mapping->vertices[3 * middleNbPoints + 6 * i + 5] = vertices[nbPoints + i];
            }
            else {
                mapping->vertices[3 * middleNbPoints + 6 * i] = vertices[nbPoints + (nbPoints + i - 1) % nbPoints];
                mapping->vertices[3 * middleNbPoints + 6 * i + 1] = vertices[i];
                mapping->vertices[3 * middleNbPoints + 6 * i + 2] = middleVertices[i * middleNbPoints / nbPoints];
                mapping->vertices[3 * middleNbPoints + 6 * i + 3] = middleVertices[i * middleNbPoints / nbPoints];
                mapping->vertices[3 * middleNbPoints + 6 * i + 4] = vertices[(nbPoints + i - 1) % nbPoints];
                mapping->vertices[3 * middleNbPoints + 6 * i + 5] = vertices[nbPoints + (nbPoints + i - 1) % nbPoints];
            }

        }

    }




}

void Gem::initializeBuffer(QOpenGLShaderProgram *shaderProgram) {

    initializeOpenGLFunctions();

    vbo = new QOpenGLBuffer[8];
    vao = new QOpenGLVertexArrayObject[8];

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


    for(int i = 0; i < 8; i++) {
        if(mappings[i].length > 0) {
            vbo[i].create();
            vbo[i].bind();
            vbo[i].setUsagePattern(QOpenGLBuffer::StaticDraw);
            vbo[i].allocate(mappings[i].vertices, mappings[i].length * sizeof(Vertex));

            vao[i].create();
            vao[i].bind();
            shaderProgram->enableAttributeArray(0);
            shaderProgram->enableAttributeArray(1);
            shaderProgram->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize,
                                              Vertex::stride());
            shaderProgram->setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize,
                                              Vertex::stride());

            vao[i].release();
            vbo[i].release();
        }
    }

}

void Gem::drawShape(QOpenGLShaderProgram *shaderProgram, int u_modelToWorld, Transform3D m_transform) {

    for(int i = 0; i < 8; i++) {
        if(mappings[i].length > 0) {
            vao[i].bind();
            shaderProgram->setUniformValue(u_modelToWorld, m_transform.toMatrix());
            glDrawArrays(mappings[i].mode, 0, mappings[i].length);
            vao[i].release();
        }
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
