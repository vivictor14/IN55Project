#ifndef IN55PROJECT_GEM_H
#define IN55PROJECT_GEM_H

#include <GL/gl.h>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QVector3D>
#include <QtGui/QOpenGLShaderProgram>
#include <src/util/transform3D.h>
#include <QtGui/QOpenGLVertexArrayObject>
#include <QtGui/QOpenGLBuffer>
#include <src/util/vertex.h>
#include <vector>

struct VerticesMapping {
    Vertex *vertices;
    int length = 0;
    QVector3D *normals;
    GLenum mode;
};

class Gem : protected QOpenGLFunctions {
private:

    GLfloat topHeight, bottomHeight;
    GLfloat topRadius, middleRadius, bottomRadius, innerMiddleRadius;
    GLint topNbPoints, middleNbPoints, bottomNbPoints, topComplexity, bottomComplexity;

    Vertex *topVertices, *middleVertices, *bottomVertices;
    VerticesMapping *mappings;

    QOpenGLBuffer *vbo;
    QOpenGLVertexArrayObject *vao;

    void calculateInnerMiddleRadius();
    float calculateComplexityCircleRadius(GLfloat height, GLfloat radius, GLint nbPoints, GLint complexity, int index);
    void initVertices(QVector3D color);
    void mapping();
    Vertex *initFaceVertices(bool counterClockWise, GLfloat height, GLfloat radius, GLint nbPoints, GLint Complexity, QVector3D color);
    void tableMapping(VerticesMapping *mapping, Vertex *vertices, GLint nbPoints, GLint complexity);
    void pavilionMapping(VerticesMapping *mapping, Vertex *vertices, GLint complexity, bool clockWise);
    void starMapping(VerticesMapping *mapping, Vertex *vertices, GLint nbPoints, GLint complexity);
    void bezelMapping(VerticesMapping *mapping, Vertex *vertices, GLint nbPoints, GLint complexity);
    void upperGirdleMapping(VerticesMapping *mapping, Vertex *vertices, GLint nbPoints, GLint complexity, bool clockWise);


public:

    Gem(GLfloat topHeight, GLfloat bottomHeight, GLfloat topRadius, GLfloat middleRadius, GLfloat bottomRadius, GLint topNbPoints, GLint middleNbPoints, GLint bottomNbPoints, GLint topComplexity, GLint bottomComplexity, QVector3D color);
    ~Gem();
    void initializeBuffer(QOpenGLShaderProgram *shaderProgram);
    void drawShape(QOpenGLShaderProgram *shaderProgram, int u_modelToWorld, Transform3D m_transform);

    void calculateNormal(VerticesMapping *pMapping);

    void normalPerVertex(Vertex *pVertex);
};


#endif //IN55PROJECT_GEM_H
