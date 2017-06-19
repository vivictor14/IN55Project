#include "displayWidget.h"

/**
 * Default constructor
 *
 */
DisplayWidget::DisplayWidget() {

    // Give the focus to the widget
    setFocusPolicy( Qt::StrongFocus );

    // Default behavior and positions
    autoRotate = true;
    m_camera.rotate(-10, 1, 0, 0);
    m_transform.translate(0, -5, -35);
    lumiere.pos = { 0.0f, 0.0f, 0.0f, 1.0f };
    lumiere.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
    lumiere.specular = { 0.5f, 0.5f, 0.5f, 1.0f };
    lumiere.diffuse = { 0.5f, 0.5f, 0.5f, 1.0f };

    // Instantiate the gem
    gem = new Gem(2, 4, 4, 6, 4, 8, 8, 8, 5, 5, 100, 100, QColor("red"));

    // Instantiate the sky box
    skyBox = new SkyBox();
}

/**
 * Destructor
 *
 */
DisplayWidget::~DisplayWidget() {
    delete shaderProgram;
    delete gem;
    delete skyBox;
}

/**
 * Initialize openGL context
 *
 */
void DisplayWidget::initializeGL() {

    // Default openGL context
    initializeOpenGLFunctions();
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Create the main shader program
    shaderProgram = new QOpenGLShaderProgram();
    shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, "..\\src\\shaders\\vertexShader.glsl");
    shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, "..\\src\\shaders\\fragmentShader.glsl");
    shaderProgram->link();

    // Set the uniform locations
    shaderProgram->bind();
    u_modelToWorld = shaderProgram->uniformLocation("modelToWorld");
    u_worldToCamera = shaderProgram->uniformLocation("worldToCamera");
    u_cameraToView = shaderProgram->uniformLocation("cameraToView");
    shaderProgram->release();

    // Initialize the gem's openGL elements
    gem->initializeBuffer(shaderProgram, &m_transform, true);

    // Create the sky box shader program
    skyBoxShaderProgram = new QOpenGLShaderProgram();
    skyBoxShaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, "..\\src\\shaders\\skyBoxVertexShader.glsl");
    skyBoxShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, "..\\src\\shaders\\skyBoxFragmentShader.glsl");
    skyBoxShaderProgram->link();

    // Initialize the sky box's openGL elements
    skyBox->initializeBuffer(skyBoxShaderProgram);

    // Connect the update function to the frame signal in order to have update called every frame
    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
}

/**
 * Draw elements
 *
 */
void DisplayWidget::paintGL() {

    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the sky box
    skyBoxShaderProgram->bind();
    skyBoxShaderProgram->setUniformValue("cameraToView", m_projection);
    skyBoxShaderProgram->setUniformValue("worldToCamera", m_camera.toMatrix());
    skyBox->drawShape();
    skyBoxShaderProgram->release();

    // Draw the gem
    shaderProgram->bind();
    shaderProgram->setUniformValue(u_worldToCamera, m_camera.toMatrix());
    shaderProgram->setUniformValue(u_cameraToView, m_projection);
    shaderProgram->setUniformValue("lumiere.pos", lumiere.pos);
    shaderProgram->setUniformValue("lumiere.ambient", lumiere.ambient);
    shaderProgram->setUniformValue("lumiere.specular", lumiere.specular);
    shaderProgram->setUniformValue("lumiere.diffuse", lumiere.diffuse);
    shaderProgram->setUniformValue("cameraPos", m_camera.translation());
    gem->drawShape(shaderProgram, u_modelToWorld, m_transform,skyBox->getTexture());
    shaderProgram->release();

}

/**
 * Resize openGL elements
 *
 * @param width
 * @param height
 */
void DisplayWidget::resizeGL(int width, int height)
{
    m_projection.setToIdentity();
    m_projection.perspective(45.0f, width / float(height), 0.0f, 1000.0f);
}

/**
 * Update function called every frame
 *
 */
void DisplayWidget::update() {

    // Update input
    Input::update();

    // Camera Transformation
    if (Input::buttonPressed(Qt::RightButton))
    {
        static const float rotSpeed = 0.5f;

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

/**
 * Key press event handler
 *
 * @param event A key press event
 */
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

/**
 * Key release event handler
 *
 * @param event A key release event
 */
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

/**
 * Mouse button press event handler
 *
 * @param event A mouse button press event
 */
void DisplayWidget::mousePressEvent(QMouseEvent *event)
{
    Input::registerMousePress(event->button());
}

/**
 * Mouse button release event handler
 *
 * @param event A mouse button release event
 */
void DisplayWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Input::registerMouseRelease(event->button());
}

/**
 * Change the displayed gem
 *
 * @param topHeight The distance between the middle and the top of the gem
 * @param bottomHeight The distance between the middle and the bottom of the gem
 * @param topRadius The radius of the top extremity
 * @param middleRadius The radius of the middle
 * @param bottomRadius The radius of the bottom extremity
 * @param topNbPoints The top number of points
 * @param middleNbPoints The middle number of points
 * @param bottomNbPoints The bottom number of points
 * @param topComplexity The factor by which the top half should be divided
 * @param bottomComplexity The factor by which the bottom half should be divided
 * @param lengthStretchingPercent The stretching percent by which the length will be multiplied
 * @param widthStretchingPercent The stretching percent by which the width will be multiplied
 * @param color The color of the gem
 */
void DisplayWidget::updateGem(GLfloat topHeight, GLfloat bottomHeight, GLfloat topRadius, GLfloat middleRadius,
                              GLfloat bottomRadius, GLint topNbPoints, GLint middleNbPoints, GLint bottomNbPoints,
                              GLint topComplexity, GLint bottomComplexity, GLint lengthStretchingPercent, GLint widthStretchingPercent, QColor color) {

    gem->initGem(topHeight, bottomHeight, topRadius, middleRadius, bottomRadius, topNbPoints, middleNbPoints,
                 bottomNbPoints, topComplexity, bottomComplexity, lengthStretchingPercent, widthStretchingPercent, color);

    gem->initializeBuffer(shaderProgram, &m_transform, false);

}

/**
 * Get the gem
 *
 * @return The gem
 */
Gem *DisplayWidget::getGem() const {
    return gem;
}

/**
 * Set the auto-rotation
 *
 * @param autoRotate 1 or 0 depending on whether or not you want the gem to rotate on itself
 */
void DisplayWidget::triggerAutoRotation(int autoRotate) {
    this->autoRotate = (bool) autoRotate;
}

/**
 * Get if the auto-rotation is on or not
 *
 * @return The auto-rotation state
 */
bool DisplayWidget::isAutoRotate() const {
    return autoRotate;
}


