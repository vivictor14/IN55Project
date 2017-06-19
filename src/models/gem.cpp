#include "gem.h"

Gem::Gem(GLfloat topHeight, GLfloat bottomHeight, GLfloat topRadius, GLfloat middleRadius, GLfloat bottomRadius,
         GLint topNbPoints, GLint middleNbPoints, GLint bottomNbPoints, GLint topComplexity, GLint bottomComplexity,
         GLint lengthStretchingPercent, GLint widthStretchingPercent, QColor color) {

    initGem(topHeight, bottomHeight, topRadius, middleRadius, bottomRadius, topNbPoints, middleNbPoints, bottomNbPoints,
            topComplexity, bottomComplexity, lengthStretchingPercent, widthStretchingPercent, color);

}

Gem::~Gem() {
    for(int i = 0; i < 4; i++) {
        vbo[i].destroy();
        vao[i].destroy();
    }
}

void Gem::initGem(GLfloat topHeight, GLfloat bottomHeight, GLfloat topRadius, GLfloat middleRadius,
                  GLfloat bottomRadius,
                  GLint topNbPoints, GLint middleNbPoints, GLint bottomNbPoints, GLint topComplexity,
                  GLint bottomComplexity,
                  GLint lengthStretchingPercent, GLint widthStretchingPercent, QColor color) {

    this->previousLengthStretchingPercent = this->lengthStretchingPercent;
    this->previousWidthStretchingPercent = this->widthStretchingPercent;

    this->topHeight = topHeight;
    this->bottomHeight = -bottomHeight;
    this->topRadius = topRadius;
    this->middleRadius = middleRadius;
    this->bottomRadius = bottomRadius;
    this->topNbPoints = topNbPoints;
    this->middleNbPoints = middleNbPoints;
    this->bottomNbPoints = bottomNbPoints;
    this->topComplexity = topComplexity;
    this->bottomComplexity = bottomComplexity;
    this->lengthStretchingPercent = lengthStretchingPercent;
    this->widthStretchingPercent = widthStretchingPercent;
    this->color = color;

    initVertices(QVector3D(color.red() / 255.0f, color.green() / 255.0f, color.blue() / 255.0f));
    mapping();
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
        float complexityCircleRadius = innerMiddleRadius - (innerMiddleRadius - radius) * (i + 1) / (float)complexity;


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

    mapping->mode = GL_TRIANGLES;
    mapping->length = nbPoints * 3 ;
    mapping->vertices = new Vertex[mapping->length];


    for(int i = 0; i < nbPoints; i++) {

        mapping->vertices[3 * i] = vertices[complexity * nbPoints - 1];
        mapping->vertices[3 * i + 1] = vertices[(complexity * nbPoints - 1) - (i + 1) % nbPoints];
        mapping->vertices[3 * i + 2] = vertices[(complexity * nbPoints - 1) - (i + 2) % nbPoints];
    }
}

void Gem::pavilionMapping(VerticesMapping *mapping, Vertex *vertices, GLint complexity, bool clockWise) {

    mapping->mode = GL_TRIANGLES;
    mapping->length  = (middleNbPoints + 2)*3;
    mapping->vertices = new Vertex[mapping->length];

    if(complexity > 1) {
        for (int i = 0; i <= middleNbPoints; i++) {

            mapping->vertices[3 * i] = vertices[middleNbPoints * (complexity - 1)];
            mapping->vertices[3 * i + 1] = vertices[(complexity - 1) * middleNbPoints - 1 - i % middleNbPoints];
            mapping->vertices[3 * i + 2] = vertices[(complexity - 1) * middleNbPoints - 1 - (i + 1) % middleNbPoints];

        }
    }
    else {
        for (int i = 0; i <= middleNbPoints; i++) {
            if(clockWise) {
                mapping->vertices[3 * i] = vertices[middleNbPoints * (complexity - 1)];
                mapping->vertices[3 * i + 1] = middleVertices[i % middleNbPoints];
                mapping->vertices[3 * i + 2] = middleVertices[(i + 1) % middleNbPoints];
            }
            else {
                mapping->vertices[3 * i] = vertices[middleNbPoints * (complexity - 1)];
                mapping->vertices[3 * i + 1] = middleVertices[(middleNbPoints - 1) - i % middleNbPoints];
                mapping->vertices[3 * i + 2] = middleVertices[(middleNbPoints - 1) - (i + 1) % middleNbPoints];
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
    mapping->length = (2 + (complexity > 1)) * 3 * nbPoints + 6 * middleNbPoints;
    mapping->vertices = new Vertex[mapping->length];
    int startIndex = 0;

    for(int i = 0; i < nbPoints; i++) {

        if(clockWise) {
            mapping->vertices[3 * i] = vertices[nbPoints - 1 - i];
            mapping->vertices[3 * i + 1] = middleVertices[(middleNbPoints - 1 + (int) round((i + 1) * middleNbPoints / (float) nbPoints)) % middleNbPoints];
            mapping->vertices[3 * i + 2] = middleVertices[(int) round((i + 1) * middleNbPoints / (float) nbPoints) % middleNbPoints];
        }
        else {
            mapping->vertices[3 * i] = vertices[i];
            mapping->vertices[3 * i + 1] = middleVertices[(int) round((i + 1) * middleNbPoints / (float) nbPoints) % middleNbPoints];
            mapping->vertices[3 * i + 2] = middleVertices[(middleNbPoints - 1 + (int) round((i + 1) * middleNbPoints / (float) nbPoints)) % middleNbPoints];
        }

    }

    startIndex = 3 * nbPoints;

    if(complexity == 1) {
        for (int i = 0; i < middleNbPoints; i++) {

            if (clockWise) {
                mapping->vertices[startIndex + 3 * i] = middleVertices[middleNbPoints - 1 - i];
                mapping->vertices[startIndex + 3 * i + 1] = vertices[(int) round((i + 1) * nbPoints / (float) middleNbPoints) - 1];
                mapping->vertices[startIndex + 3 * i + 2] = vertices[(int) round((i + 1) * nbPoints / (float) middleNbPoints) % nbPoints];
            } else {
                mapping->vertices[startIndex + 3 * i] = middleVertices[(i + 1) % middleNbPoints];
                mapping->vertices[startIndex + 3 * i + 2] = vertices[(int) round((i + 1) * nbPoints / (float) middleNbPoints) % nbPoints];
                mapping->vertices[startIndex + 3 * i + 1] = vertices[(int) round((i + 1) * nbPoints / (float) middleNbPoints) - 1];
            }

        }
    }

    startIndex = 3 * nbPoints + 3 * middleNbPoints;

    for(int i = 0; i < middleNbPoints; i++) {

        if(clockWise){
            mapping->vertices[startIndex + 3 * i] = middleVertices[i];
            mapping->vertices[startIndex + 3 * i + 1] = middleVertices[(i + 1) % middleNbPoints];
            mapping->vertices[startIndex + 3 * i + 2] = vertices[nbPoints - 1 - (int)round(i * nbPoints / (float)middleNbPoints) % nbPoints];
        }
        else {
            mapping->vertices[startIndex + 3 * i] = middleVertices[(i + 1) % middleNbPoints];
            mapping->vertices[startIndex + 3 * i + 1] = middleVertices[i];
            mapping->vertices[startIndex + 3 * i + 2] = vertices[(int)round(i * nbPoints / (float)middleNbPoints) % nbPoints];
        }

    }

    startIndex = 3 * nbPoints + 6 * middleNbPoints;

    if(complexity == 1) {
        for (int i = 0; i < nbPoints; i++) {

            if(clockWise) {
                mapping->vertices[startIndex + 3 * i] = vertices[nbPoints - 1 - i];
                mapping->vertices[startIndex + 3 * i + 1] = vertices[(nbPoints - i) % nbPoints];
                mapping->vertices[startIndex + 3 * i + 2] = middleVertices[((int)round(i * middleNbPoints / (float)nbPoints)) % middleNbPoints];
            }
            else {
                mapping->vertices[startIndex + 3 * i] = vertices[(nbPoints + i - 1) % nbPoints ];
                mapping->vertices[startIndex + 3 * i + 1] = vertices[i];
                mapping->vertices[startIndex + 3 * i + 2] = middleVertices[((int)round(i * middleNbPoints / (float)nbPoints)) % middleNbPoints];
            }

        }
    }
    else {

        for (int i = 0; i < nbPoints; i++) {

            if(clockWise) {
                mapping->vertices[startIndex + 6 * i] = vertices[nbPoints + (nbPoints - i - 1) % nbPoints];
                mapping->vertices[startIndex + 6 * i + 1] = vertices[(nbPoints - i) % nbPoints];
                mapping->vertices[startIndex + 6 * i + 2] = middleVertices[(int)round(i * middleNbPoints / (float)nbPoints) % middleNbPoints];
                mapping->vertices[startIndex + 6 * i + 3] = middleVertices[(int)round(i * middleNbPoints / (float)nbPoints) % middleNbPoints];
                mapping->vertices[startIndex + 6 * i + 4] = vertices[nbPoints - 1 - i];
                mapping->vertices[startIndex + 6 * i + 5] = vertices[nbPoints + (nbPoints - i - 1) % nbPoints];
            }
            else {
                mapping->vertices[startIndex + 6 * i] = vertices[nbPoints + (nbPoints + i - 1) % nbPoints];
                mapping->vertices[startIndex + 6 * i + 1] = vertices[i];
                mapping->vertices[startIndex + 6 * i + 2] = middleVertices[(int)round(i * middleNbPoints / (float)nbPoints) % middleNbPoints];
                mapping->vertices[startIndex + 6 * i + 3] = middleVertices[(int)round(i * middleNbPoints / (float)nbPoints) % middleNbPoints];
                mapping->vertices[startIndex + 6 * i + 4] = vertices[(nbPoints + i - 1) % nbPoints];
                mapping->vertices[startIndex + 6 * i + 5] = vertices[nbPoints + (nbPoints + i - 1) % nbPoints];
            }

        }

    }




}

void Gem::initializeBuffer(QOpenGLShaderProgram *shaderProgram, Transform3D *m_transform, bool init) {

    initializeOpenGLFunctions();

    if(init) {
        shaderProgram->bind();
        vbo = new QOpenGLBuffer[8];
        vao = new QOpenGLVertexArrayObject[8];
    }

    m_transform->scale(lengthStretchingPercent / (float)previousLengthStretchingPercent, 1,
                       widthStretchingPercent / (float)previousWidthStretchingPercent);

    for(int i=0;i<8;i++) {
        calculateNormal(&mappings[i]);
    }

    for(int i = 0; i < 8; i++) {
        if(mappings[i].length > 0) {
            if(init) {
                vbo[i].create();
            }

            vbo[i].bind();
            vbo[i].setUsagePattern(QOpenGLBuffer::StaticDraw);
            vbo[i].allocate(mappings[i].vertices, mappings[i].length * sizeof(Vertex));

            if(init) {
                vao[i].create();
            }

            vao[i].bind();
            shaderProgram->enableAttributeArray(0);
            shaderProgram->enableAttributeArray(1);
            shaderProgram->enableAttributeArray(2);
            shaderProgram->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
            shaderProgram->setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());
            shaderProgram->setAttributeBuffer(2, GL_FLOAT, Vertex::normalOffset(), Vertex::NormalTupleSize, Vertex::stride());

            vao[i].release();
            vbo[i].release();
        }
    }
    if(init)
        shaderProgram->release();

}

void Gem::drawShape(QOpenGLShaderProgram *shaderProgram, int u_modelToWorld, Transform3D m_transform,unsigned int cubemapTexture) {

    for(int i = 0; i < 8; i++) {
        if(mappings[i].length > 0) {
            vao[i].bind();
            shaderProgram->setUniformValue(u_modelToWorld, m_transform.toMatrix());
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
            glDepthMask(GL_TRUE);
            glDrawArrays(mappings[i].mode, 0, mappings[i].length);
            vao[i].release();
        }
    }

}

void Gem::calculateNormal(VerticesMapping *Mapping) {

//    Mapping->normals = new QVector3D[Mapping->length];
    Vertex point1, point2,point3;
    float x,y,z;
    QVector3D *u,*v,*n;

    switch(Mapping->mode){
        case GL_TRIANGLE_FAN:
            point1 = Mapping->vertices[0];
//            Mapping->normals[0] = *(new QVector3D(0.f,0.f,0.f));
            for(int i=1;i<Mapping->length-1;i++){

                point2 = Mapping->vertices[i];
                point3 = Mapping->vertices[i+1];
                u = new QVector3D(point2.position()-point1.position());
                v = new QVector3D(point3.position()-point1.position());
                x = (u->y()*v->z() - (u->z()*v->y()));
                y = (u->z()*v->x() -(u->x()*v->z()));
                z = (u->x()*v->y() - (u->y()*v->x()));
                n = new QVector3D(x,y,z);
                Mapping->vertices[0].setNormal(*n);
                Mapping->vertices[i].setNormal(*n);
                Mapping->vertices[i + 1].setNormal(*n);
            }
            point2 = Mapping->vertices[Mapping->length-1];
            point3 = Mapping->vertices[1];
            u = new QVector3D(point2.position()-point1.position());
            v = new QVector3D(point3.position()-point1.position());
            x = (u->y()*v->z() - (u->z()*v->y()));
            y = (u->z()*v->x() -(u->x()*v->z()));
            z = (u->x()*v->y() - (u->y()*v->x()));
            n = new QVector3D(x,y,z);
            Mapping->vertices[0].setNormal(*n);
            Mapping->vertices[Mapping->length - 1].setNormal(*n);
            Mapping->vertices[1].setNormal(*n);
            break;

        case GL_TRIANGLES:
        default:
            for(int i=0;i<Mapping->length;i=i+3){
                point1 = Mapping->vertices[i];
                point2 = Mapping->vertices[i+1];
                point3 = Mapping->vertices[i+2];
                u = new QVector3D(point2.position()-point1.position());
                v = new QVector3D(point3.position()-point1.position());
                x = (u->y()*v->z() - (u->z()*v->y()));
                y = (u->z()*v->x() -(u->x()*v->z()));
                z = (u->x()*v->y() - (u->y()*v->x()));
                n = new QVector3D(x,y,z);
                Mapping->vertices[i].setNormal(*n);
                Mapping->vertices[i + 1].setNormal(*n);
                Mapping->vertices[i + 2].setNormal(*n);
            }

    }

}

GLfloat Gem::getTopHeight() const {
    return topHeight;
}

GLfloat Gem::getBottomHeight() const {
    return -bottomHeight;
}

GLfloat Gem::getTopRadius() const {
    return topRadius;
}

GLfloat Gem::getMiddleRadius() const {
    return middleRadius;
}

GLfloat Gem::getBottomRadius() const {
    return bottomRadius;
}

GLint Gem::getTopNbPoints() const {
    return topNbPoints;
}

GLint Gem::getMiddleNbPoints() const {
    return middleNbPoints;
}

GLint Gem::getBottomNbPoints() const {
    return bottomNbPoints;
}

GLint Gem::getTopComplexity() const {
    return topComplexity;
}

GLint Gem::getBottomComplexity() const {
    return bottomComplexity;
}

const QColor &Gem::getColor() const {
    return color;
}


GLint Gem::getLengthStretchingPercent() const {
    return lengthStretchingPercent;
}

GLint Gem::getWidthStretchingPercent() const {
    return widthStretchingPercent;
}
