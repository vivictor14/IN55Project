#include <QtGui/QOpenGLVertexArrayObject>
#include <QtGui/QOpenGLBuffer>
#include <src/util/vertex.h>
#include "gem.h"

// Front Verticies
#define VERTEX_FTR Vertex( QVector3D( 0.5f,  0.5f,  0.5f), QVector3D( 1.0f, 0.0f, 0.0f ) )
#define VERTEX_FTL Vertex( QVector3D(-0.5f,  0.5f,  0.5f), QVector3D( 0.0f, 1.0f, 0.0f ) )
#define VERTEX_FBL Vertex( QVector3D(-0.5f, -0.5f,  0.5f), QVector3D( 0.0f, 0.0f, 1.0f ) )
#define VERTEX_FBR Vertex( QVector3D( 0.5f, -0.5f,  0.5f), QVector3D( 0.0f, 0.0f, 0.0f ) )

// Back Verticies
#define VERTEX_BTR Vertex( QVector3D( 0.5f,  0.5f, -0.5f), QVector3D( 1.0f, 1.0f, 0.0f ) )
#define VERTEX_BTL Vertex( QVector3D(-0.5f,  0.5f, -0.5f), QVector3D( 0.0f, 1.0f, 1.0f ) )
#define VERTEX_BBL Vertex( QVector3D(-0.5f, -0.5f, -0.5f), QVector3D( 1.0f, 0.0f, 1.0f ) )
#define VERTEX_BBR Vertex( QVector3D( 0.5f, -0.5f, -0.5f), QVector3D( 1.0f, 1.0f, 1.0f ) )

// Create a colored cube
static const Vertex sg_vertexes[] = {
        // Face 1 (Front)
        VERTEX_FTR, VERTEX_FTL, VERTEX_FBL,
        VERTEX_FBL, VERTEX_FBR, VERTEX_FTR,
        // Face 2 (Back)
        VERTEX_BBR, VERTEX_BTL, VERTEX_BTR,
        VERTEX_BTL, VERTEX_BBR, VERTEX_BBL,
        // Face 3 (Top)
        VERTEX_FTR, VERTEX_BTR, VERTEX_BTL,
        VERTEX_BTL, VERTEX_FTL, VERTEX_FTR,
        // Face 4 (Bottom)
        VERTEX_FBR, VERTEX_FBL, VERTEX_BBL,
        VERTEX_BBL, VERTEX_BBR, VERTEX_FBR,
        // Face 5 (Left)
        VERTEX_FBL, VERTEX_FTL, VERTEX_BTL,
        VERTEX_FBL, VERTEX_BTL, VERTEX_BBL,
        // Face 6 (Right)
        VERTEX_FTR, VERTEX_FBR, VERTEX_BBR,
        VERTEX_BBR, VERTEX_BTR, VERTEX_FTR
};

#undef VERTEX_BBR
#undef VERTEX_BBL
#undef VERTEX_BTL
#undef VERTEX_BTR

#undef VERTEX_FBR
#undef VERTEX_FBL
#undef VERTEX_FTL
#undef VERTEX_FTR

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
    buffer.destroy();
    vao.destroy();
}

void Gem::initVertices(QVector3D color) {
    topVertices = new Vertex [ topComplexity * topNbPoints ];
    middleVertices = new Vertex [ middleNbPoints ];
    bottomVertices = new Vertex [ bottomComplexity * bottomNbPoints ];

    double angle = 2.0f * M_PI / middleNbPoints;
    for(int i = 0; i < middleNbPoints; i++) {
        float x = (float) cos(i * angle) * middleRadius;
        float z = (float) sin(i * angle) * middleRadius;
        middleVertices[i] = Vertex( QVector3D(x, 0, z), color);
    }
    calculateInnerMiddleRadius();

    initFaceVertices(topVertices, topHeight, topRadius, topNbPoints, topComplexity, color);
    initFaceVertices(bottomVertices, bottomHeight, bottomRadius, bottomNbPoints, bottomComplexity, color);

}

void Gem::calculateInnerMiddleRadius() {

    QVector3D A = middleVertices[0].position();
    QVector3D B = middleVertices[1].position();
    QVector3D M = QVector3D((A.x() + B.x()) / 2, (A.y() + B.y()) / 2, (A.z() + B.z()) / 2);
    innerMiddleRadius = (GLfloat) sqrt(pow(M.x(), 2) + pow(M.y(), 2) + pow(M.z(), 2));

}

void Gem::initFaceVertices(Vertex *vertices, GLfloat height, GLfloat radius, GLint nbPoints, GLint complexity, QVector3D color) {

    double angle = 2.0f * M_PI / nbPoints;
    double offset;

    for(int i = 0; i < complexity; i++) {

        if((complexity - i)%2 != 0) {
            offset = angle / 2;
        }
        else {
            offset = 0;
        }

        for(int j = 0; j < nbPoints; j++) {

            float x, y, z;
            float complexityCircleRadius = radius + i * ((innerMiddleRadius - radius) / complexity);
            x = (float) cos(i * angle + offset) * complexityCircleRadius;
            z = (float) sin(i * angle + offset) * complexityCircleRadius;
            y = height - i * (height / complexity);
            vertices[(i * nbPoints) + j] = Vertex(QVector3D(x, y, z), color);

        }

    }

}

void Gem::initMapping() {

}

void Gem::initializeBuffer(QOpenGLShaderProgram *shaderProgram) {

    initializeOpenGLFunctions();

    buffer.create();
    buffer.bind();
    buffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    buffer.allocate(sg_vertexes, sizeof(sg_vertexes));

    vao.create();
    vao.bind();
    shaderProgram->enableAttributeArray(0);
    shaderProgram->enableAttributeArray(1);
    shaderProgram->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
    shaderProgram->setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());

    vao.release();
    buffer.release();
}

void Gem::drawShape(QOpenGLShaderProgram *shaderProgram, int u_modelToWorld, Transform3D m_transform) {

    vao.bind();
    shaderProgram->setUniformValue(u_modelToWorld, m_transform.toMatrix());
    glDrawArrays(GL_TRIANGLES, 0, sizeof(sg_vertexes) / sizeof(sg_vertexes[0]));
    vao.release();

}
