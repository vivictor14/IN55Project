#ifndef IN55PROJECT_DISPLAYWINDOW_H
#define IN55PROJECT_DISPLAYWINDOW_H

#include <QOpenGlShader>
#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include "vertex.h"

class QOpenGLShaderProgram;

class DisplayWindow : public QOpenGLWindow, protected QOpenGLFunctions {

    Q_OBJECT

public:

    DisplayWindow();
    ~DisplayWindow();

    void initializeGL();
//    void resizeGL(int width, int height);
    void paintGL();
    void teardownGL();

private:

    QOpenGLShaderProgram *shaderProgram;
    QOpenGLBuffer buffer;
    QOpenGLVertexArrayObject vao;

    void keyPressEvent(QKeyEvent *event);

};


#endif //IN55PROJECT_DISPLAYWINDOW_H
