#ifndef IN55PROJECT_DISPLAYWINDOW_H
#define IN55PROJECT_DISPLAYWINDOW_H

#include <QOpenGlShader>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include "vertex.h"
#include "gem.h"
#include "SkyBox.h"
#include <QMatrix4x4>
#include <QtWidgets/QOpenGLWidget>
#include "transform3d.h"
#include "camera3d.h"
#include "input.h"
#include <QKeyEvent>
#include <QPainter>

class DisplayWidget : public QOpenGLWidget, protected QOpenGLFunctions {

    Q_OBJECT

private:

    Gem *gem;
    SkyBox *skyBox;
    QOpenGLShaderProgram *shaderProgram;
    QOpenGLShaderProgram *skyBoxShaderProgram;
    bool autoRotate = true;
    int u_modelToWorld;
    int u_worldToCamera;
    int u_cameraToView;
    QMatrix4x4 m_projection;
    Camera3D m_camera;
    Transform3D m_transform;
    struct
    {
        QVector4D pos;
        QVector4D ambient;
        QVector4D diffuse;
        QVector4D specular;
    }lumiere;

public:

    DisplayWidget();

    ~DisplayWidget();

    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void updateGem(GLfloat topHeight, GLfloat bottomHeight, GLfloat topRadius, GLfloat middleRadius,
                   GLfloat bottomRadius, GLint topNbPoints, GLint middleNbPoints, GLint bottomNbPoints,
                   GLint topComplexity, GLint bottomComplexity, GLint lengthStretchingPercent, GLint widthStretchingPercent, QColor color);

    Gem *getGem() const;

    bool isAutoRotate() const;

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

protected slots:

    void update();
    void triggerAutoRotation(int autoRotate);

};


#endif //IN55PROJECT_DISPLAYWINDOW_H
