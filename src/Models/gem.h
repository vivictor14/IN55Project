#ifndef IN55PROJECT_GEM_H
#define IN55PROJECT_GEM_H

#include <GL/gl.h>

class Gem {
private:
    GLfloat topHeight, bottomHeight;
    GLfloat topRadius, middleRadius, bottomRadius;
    GLint topNbPoints, middleNbPoints, bottomNbPoints;

    GLfloat *colors, *vertices, *mapping;

    void initVertices();
    void initMapping();
    void initColors(GLfloat *color);
public:
    Gem(GLfloat topHeight, GLfloat bottomHeight, GLfloat topRadius, GLfloat middleRadius, GLfloat bottomRadius, GLint topNbPoints, GLint middleNbPoints, GLint bottomNbPoints, GLfloat *color);
    ~Gem();
    void drawShape();
};


#endif //IN55PROJECT_GEM_H
