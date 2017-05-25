#ifndef IN55PROJECT_GEM_H
#define IN55PROJECT_GEM_H

#include <GL/gl.h>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QVector3D>
#include <QtGui/QOpenGLShaderProgram>
#include <src/util/transform3D.h>

class Gem : protected QOpenGLFunctions {
private:

    GLfloat topHeight, bottomHeight;
    GLfloat topRadius, middleRadius, bottomRadius, innerMiddleRadius;
    GLint topNbPoints, middleNbPoints, bottomNbPoints, topComplexity, bottomComplexity;

    Vertex *topVertices, *middleVertices, *bottomVertices;
    Vertex **drawVertices;

    QOpenGLBuffer *vbo;
    QOpenGLVertexArrayObject *vao;

    void calculateInnerMiddleRadius();
    void initVertices(QVector3D color);
    void initMapping();
    Vertex *initFaceVertices(GLfloat height, GLfloat radius, GLint nbPoints, GLint Complexity, QVector3D color);
    Vertex *initFanFaceMapping(Vertex *vertices, GLint nbPoints, GLint complexity);
    Vertex *initStripFaceMapping(Vertex *vertices, GLint nbPoints, GLint complexity);
    Vertex *initTrianglesFaceMapping(Vertex *vertices, GLint nbPoints, GLint complexity);

public:

    Gem(GLfloat topHeight, GLfloat bottomHeight, GLfloat topRadius, GLfloat middleRadius, GLfloat bottomRadius, GLint topNbPoints, GLint middleNbPoints, GLint bottomNbPoints, GLint topComplexity, GLint bottomComplexity, QVector3D color);
    ~Gem();
    void initializeBuffer(QOpenGLShaderProgram *shaderProgram);
    void drawShape(QOpenGLShaderProgram *shaderProgram, int u_modelToWorld, Transform3D m_transform);



};


#endif //IN55PROJECT_GEM_H
