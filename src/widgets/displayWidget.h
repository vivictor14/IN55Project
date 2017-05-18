#ifndef IN55PROJECT_DISPLAYWINDOW_H
#define IN55PROJECT_DISPLAYWINDOW_H

#include <QOpenGlShader>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include "vertex.h"
#include "gem.h"
#include <QMatrix4x4>
#include <QtWidgets/QOpenGLWidget>
#include "transform3d.h"
#include "camera3d.h"
#include "input.h"
#include <QKeyEvent>

class QOpenGLShaderProgram;

class DisplayWidget : public QOpenGLWidget, protected QOpenGLFunctions {

    Q_OBJECT

private:

    Gem *gem;
    QOpenGLShaderProgram *shaderProgram;
    int u_modelToWorld;
    int u_worldToCamera;
    int u_cameraToView;
    QMatrix4x4 m_projection;
    Camera3D m_camera;
    Transform3D m_transform;

public:

    DisplayWidget();

    ~DisplayWidget();

    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

protected slots:

    void update();


};


#endif //IN55PROJECT_DISPLAYWINDOW_H
