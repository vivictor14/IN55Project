#include "gem.h"

Gem::Gem(GLfloat topHeight, GLfloat bottomHeight, GLfloat topRadius, GLfloat middleRadius, GLfloat bottomRadius, GLint topNbPoints, GLint middleNbPoints,
         GLint bottomNbPoints, GLfloat *color) {
    this->topHeight = topHeight;
    this->bottomHeight = bottomHeight;
    this->topRadius = topRadius;
    this->middleRadius = middleRadius;
    this->bottomRadius = bottomRadius;
    this->topNbPoints = topNbPoints;
    this->middleNbPoints = middleNbPoints;
    this->bottomNbPoints = bottomNbPoints;

    initVertices();
    initMapping();
    initColors(color);
}

void Gem::initVertices() {

}

void Gem::initMapping() {

}

void Gem::initColors(GLfloat *color) {

}

void Gem::drawShape() {
//    glDrawElements(GL_TRIANGLE_FAN, 2 * n + 2, GL_UNSIGNED_INT, g_TabPositionsStar );
//    glDrawElements(GL_TRIANGLE_FAN, 2 * n + 2, GL_UNSIGNED_INT, g_TabPositionsStar2 );
//    glDrawElements(GL_TRIANGLE_STRIP, 4 * n + 2, GL_UNSIGNED_INT, g_TabPositionsStar3);
}