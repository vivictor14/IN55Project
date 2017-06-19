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
#include <QtGui/QColor>

struct VerticesMapping {
    Vertex *vertices;
    int length = 0;
    GLenum mode;
};

class Gem : protected QOpenGLFunctions {
private:

    QColor color;

    GLfloat topHeight, bottomHeight;
    GLfloat topRadius, middleRadius, bottomRadius, innerMiddleRadius;
    GLint topNbPoints, middleNbPoints, bottomNbPoints;
    GLint topComplexity, bottomComplexity;
    GLint lengthStretchingPercent = 100;
    GLint widthStretchingPercent = 100;

    GLint previousLengthStretchingPercent;
    GLint previousWidthStretchingPercent;

    Vertex *topVertices, *middleVertices, *bottomVertices;
    VerticesMapping *mappings;

    QOpenGLBuffer *vbo;
    QOpenGLVertexArrayObject *vao;

    void calculateInnerMiddleRadius();
    void initVertices(QVector3D color);
    void mapping();
    Vertex *initFaceVertices(bool counterClockWise, GLfloat height, GLfloat radius, GLint nbPoints, GLint Complexity, QVector3D color);
    void tableMapping(VerticesMapping *mapping, Vertex *vertices, GLint nbPoints, GLint complexity);
    void pavilionMapping(VerticesMapping *mapping, Vertex *vertices, GLint complexity, bool clockWise);
    void starMapping(VerticesMapping *mapping, Vertex *vertices, GLint nbPoints, GLint complexity);
    void bezelMapping(VerticesMapping *mapping, Vertex *vertices, GLint nbPoints, GLint complexity);
    void upperGirdleMapping(VerticesMapping *mapping, Vertex *vertices, GLint nbPoints, GLint complexity, bool clockWise);
    void calculateNormal(VerticesMapping *pMapping);

public:

    Gem(GLfloat topHeight, GLfloat bottomHeight, GLfloat topRadius, GLfloat middleRadius, GLfloat bottomRadius,
        GLint topNbPoints, GLint middleNbPoints, GLint bottomNbPoints, GLint topComplexity, GLint bottomComplexity,
        GLint lengthStretchingPercent, GLint widthStretchingPercent, QColor color);
    ~Gem();
    void initializeBuffer(QOpenGLShaderProgram *shaderProgram, Transform3D *m_transform, bool init);


    void initGem(GLfloat topHeight, GLfloat bottomHeight, GLfloat topRadius, GLfloat middleRadius, GLfloat bottomRadius,
                 GLint topNbPoints, GLint middleNbPoints, GLint bottomNbPoints, GLint topComplexity, GLint bottomComplexity,
                 GLint lengthStretchingPercent, GLint widthStretchingPercent, QColor color);
    void drawShape(QOpenGLShaderProgram *shaderProgram, int u_modelToWorld, Transform3D m_transform,unsigned int cubemapTexture);

    // Accessors
    GLfloat getTopHeight() const;
    GLfloat getBottomHeight() const;
    GLfloat getTopRadius() const;
    GLfloat getMiddleRadius() const;
    GLfloat getBottomRadius() const;
    GLint getTopNbPoints() const;
    GLint getMiddleNbPoints() const;
    GLint getBottomNbPoints() const;
    GLint getTopComplexity() const;
    GLint getBottomComplexity() const;
    GLint getLengthStretchingPercent() const;
    GLint getWidthStretchingPercent() const;
    const QColor &getColor() const;
};


#endif //IN55PROJECT_GEM_H
