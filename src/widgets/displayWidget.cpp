#include "displayWidget.h"

DisplayWidget::DisplayWidget() {
    setFocusPolicy( Qt::StrongFocus );
    m_camera.rotate(-40, 1, 0, 0);
    m_transform.translate(0, -20, -20);
    gem = new Gem(2, -4, 4, 5, 8, 8, 8, 4, 4, 3, QVector3D(0.0f, 1.0f, 1.0f));
}

DisplayWidget::~DisplayWidget() {
    makeCurrent();
    delete shaderProgram;
    delete gem;
}

void DisplayWidget::initializeGL() {

    initializeOpenGLFunctions();
    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));

    glEnable(GL_CULL_FACE);
    glEnable (GL_POINT);

    //Active la transparence
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    shaderProgram = new QOpenGLShaderProgram();
    shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, "..\\src\\shaders\\base\\vertexShader.glsl");
    shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, "..\\src\\shaders\\base\\fragmentShader.glsl");

    shaderProgram->link();

    shaderProgram->bind();
    u_modelToWorld = shaderProgram->uniformLocation("modelToWorld");
    u_worldToCamera = shaderProgram->uniformLocation("worldToCamera");
    u_cameraToView = shaderProgram->uniformLocation("cameraToView");

    gem->initializeBuffer(shaderProgram);

    shaderProgram->release();

}

void DisplayWidget::paintGL() {

    glClear(GL_COLOR_BUFFER_BIT);

    shaderProgram->bind();
    shaderProgram->setUniformValue(u_worldToCamera, m_camera.toMatrix());
    shaderProgram->setUniformValue(u_cameraToView, m_projection);
    gem->drawShape(shaderProgram, u_modelToWorld, m_transform);
    shaderProgram->release();

}

void DisplayWidget::resizeGL(int width, int height)
{
    m_projection.setToIdentity();
    m_projection.perspective(45.0f, width / float(height), 0.0f, 1000.0f);
}

void DisplayWidget::update() {

    // Update input
    Input::update();

    // Camera Transformation
    if (Input::buttonPressed(Qt::RightButton))
    {
        static const float transSpeed = 0.5f;
        static const float rotSpeed   = 0.5f;

        // Handle rotations
        m_camera.rotate(-rotSpeed * Input::mouseDelta().x(), Camera3D::LocalUp);
        m_camera.rotate(-rotSpeed * Input::mouseDelta().y(), m_camera.right());

        // Handle translations
        QVector3D translation;
        if (Input::keyPressed(Qt::Key_Z))
        {
            translation += m_camera.forward();
        }
        if (Input::keyPressed(Qt::Key_S))
        {
            translation -= m_camera.forward();
        }
        if (Input::keyPressed(Qt::Key_Q))
        {
            translation -= m_camera.right();
        }
        if (Input::keyPressed(Qt::Key_D))
        {
            translation += m_camera.right();
        }
        if (Input::keyPressed(Qt::Key_A))
        {
            translation -= m_camera.up();
        }
        if (Input::keyPressed(Qt::Key_E))
        {
            translation += m_camera.up();
        }
        m_camera.translate(transSpeed * translation);
    }

    // Update instance information
//    m_transform.rotate(1.0f, QVector3D(0.4f, 0.3f, 0.3f));

    // Schedule a redraw
    QOpenGLWidget::update();
}

void DisplayWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
    {
        event->ignore();
    }
    else
    {
        Input::registerKeyPress(event->key());
    }
}

void DisplayWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
    {
        event->ignore();
    }
    else
    {
        Input::registerKeyRelease(event->key());
    }
}

void DisplayWidget::mousePressEvent(QMouseEvent *event)
{
    Input::registerMousePress(event->button());
}

void DisplayWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Input::registerMouseRelease(event->button());
}
