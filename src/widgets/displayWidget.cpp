#include "displayWidget.h"

DisplayWidget::DisplayWidget() {
    setFocusPolicy( Qt::StrongFocus );
    m_camera.rotate(-40, 1, 0, 0);
    m_transform.translate(0, -20, -20);
    lumiere.pos = {0.0f,0.0f,0.0f,1.0f};
    lumiere.ambiant = {0.5f, 0.5f, 0.5f, 1.0f };
    lumiere.specular = {0.0f, 0.0f, 1.0f, 1.0f };
    lumiere.diffuse = {0.5f, 0.5f, 0.5f, 1.0f };
    gem = new Gem(2, 4, 4, 6, 4, 8, 8, 8, 5, 5, 100, 100, QColor("red"));
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

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    shaderProgram = new QOpenGLShaderProgram();
    shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, "..\\src\\shaders\\base\\vertexShader.glsl");
    shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, "..\\src\\shaders\\base\\fragmentShader.glsl");

    shaderProgram->link();

    shaderProgram->bind();
    u_modelToWorld = shaderProgram->uniformLocation("modelToWorld");
    u_worldToCamera = shaderProgram->uniformLocation("worldToCamera");
    u_cameraToView = shaderProgram->uniformLocation("cameraToView");
    shaderProgram->release();

    gem->initializeBuffer(shaderProgram, &m_transform, true);

}

void DisplayWidget::paintGL() {

    glClear(GL_COLOR_BUFFER_BIT);

    shaderProgram->bind();
    shaderProgram->setUniformValue(u_worldToCamera, m_camera.toMatrix());
    shaderProgram->setUniformValue(u_cameraToView, m_projection);
    shaderProgram->setUniformValue("lumiere.pos", lumiere.pos);
    shaderProgram->setUniformValue("lumiere.ambiant", lumiere.ambiant);
    shaderProgram->setUniformValue("lumiere.specular", lumiere.specular);
    shaderProgram->setUniformValue("lumiere.diffuse", lumiere.diffuse);
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

        static const float rotSpeed   = 0.5f;

        // Handle rotations
        m_camera.rotate(-rotSpeed * Input::mouseDelta().x(), Camera3D::LocalUp);
        m_camera.rotate(-rotSpeed * Input::mouseDelta().y(), m_camera.right());


    }

    static const float transSpeed = 0.5f;

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
    if (Input::keyPressed(Qt::Key_L))
    {
        lumiere.pos.setZ(m_camera.translation().z()) ;
        lumiere.pos.setY(m_camera.translation().y()) ;
        lumiere.pos.setX(m_camera.translation().x()) ;
    }
    m_camera.translate(transSpeed * translation);

    // Update instance information
    if(autoRotate) {
        m_transform.rotate(1.0f, QVector3D(0.4f, 0.3f, 0.3f));
    }

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

void DisplayWidget::updateGem(GLfloat topHeight, GLfloat bottomHeight, GLfloat topRadius, GLfloat middleRadius,
                              GLfloat bottomRadius, GLint topNbPoints, GLint middleNbPoints, GLint bottomNbPoints,
                              GLint topComplexity, GLint bottomComplexity, GLint lengthStretchingPercent, GLint widthStretchingPercent, QColor color) {

    gem->initGem(topHeight, bottomHeight, topRadius, middleRadius, bottomRadius, topNbPoints, middleNbPoints,
                 bottomNbPoints, topComplexity, bottomComplexity, lengthStretchingPercent, widthStretchingPercent, color);

    gem->initializeBuffer(shaderProgram, &m_transform, false);

}

Gem *DisplayWidget::getGem() const {
    return gem;
}

void DisplayWidget::triggerAutoRotation(int autoRotate) {
    this->autoRotate = (bool) autoRotate;
}

bool DisplayWidget::isAutoRotate() const {
    return autoRotate;
}


