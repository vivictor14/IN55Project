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
    GLfloat topRadius, middleRadius, bottomRadius;
    GLint topNbPoints, middleNbPoints, bottomNbPoints;

    GLfloat *colors, *vertices, *mapping;

    QOpenGLBuffer buffer;
    QOpenGLVertexArrayObject vao;

    void initVertices(QVector3D color);
    void initMapping();

public:

    Gem(GLfloat topHeight, GLfloat bottomHeight, GLfloat topRadius, GLfloat middleRadius, GLfloat bottomRadius, GLint topNbPoints, GLint middleNbPoints, GLint bottomNbPoints, QVector3D color);
    ~Gem();
    void initializeBuffer(QOpenGLShaderProgram *shaderProgram);
    void drawShape(QOpenGLShaderProgram *shaderProgram, int u_modelToWorld, Transform3D m_transform);

};


#endif //IN55PROJECT_GEM_H
