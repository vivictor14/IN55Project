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
    cameraMatrix.rotate(-10, 1, 0, 0);
    transformMatrix.translate(0, -5, -35);
    light.position = { 0.0f, 0.0f, 0.0f, 1.0f };
    light.ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
    light.specular = { 0.5f, 0.5f, 0.5f, 1.0f };
    light.diffuse = { 0.5f, 0.5f, 0.5f, 1.0f };

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
    modelToWorldLocation = shaderProgram->uniformLocation("modelToWorld");
    worldToCameraLocation = shaderProgram->uniformLocation("worldToCamera");
    cameraToViewLocation = shaderProgram->uniformLocation("cameraToView");
    shaderProgram->release();

    // Initialize the gem's openGL elements
    gem->initializeBuffer(shaderProgram, &transformMatrix, true);

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
    skyBoxShaderProgram->setUniformValue("cameraToView", projectionMatrix);
    skyBoxShaderProgram->setUniformValue("worldToCamera", cameraMatrix.toMatrix());
    skyBox->drawShape();
    skyBoxShaderProgram->release();

    // Draw the gem
    shaderProgram->bind();
    shaderProgram->setUniformValue(worldToCameraLocation, cameraMatrix.toMatrix());
    shaderProgram->setUniformValue(cameraToViewLocation, projectionMatrix);
    shaderProgram->setUniformValue("light.position", light.position);
    shaderProgram->setUniformValue("light.ambient", light.ambient);
    shaderProgram->setUniformValue("light.specular", light.specular);
    shaderProgram->setUniformValue("light.diffuse", light.diffuse);
    shaderProgram->setUniformValue("cameraPos", cameraMatrix.translation());
    gem->drawShape(shaderProgram, modelToWorldLocation, transformMatrix,skyBox->getTexture());
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
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(45.0f, width / float(height), 0.0f, 1000.0f);
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
        cameraMatrix.rotate(-rotSpeed * Input::mouseDelta().x(), Camera3D::LocalUp);
        cameraMatrix.rotate(-rotSpeed * Input::mouseDelta().y(), cameraMatrix.right());
    }

    static const float transSpeed = 0.5f;

    // Handle translations
    QVector3D translation;
    if (Input::keyPressed(Qt::Key_Z))
    {
        translation += cameraMatrix.forward();
    }
    if (Input::keyPressed(Qt::Key_S))
    {
        translation -= cameraMatrix.forward();
    }
    if (Input::keyPressed(Qt::Key_Q))
    {
        translation -= cameraMatrix.right();
    }
    if (Input::keyPressed(Qt::Key_D))
    {
        translation += cameraMatrix.right();
    }
    if (Input::keyPressed(Qt::Key_A))
    {
        translation -= cameraMatrix.up();
    }
    if (Input::keyPressed(Qt::Key_E))
    {
        translation += cameraMatrix.up();
    }
    if (Input::keyPressed(Qt::Key_L))
    {
        light.position.setZ(cameraMatrix.translation().z()) ;
        light.position.setY(cameraMatrix.translation().y()) ;
        light.position.setX(cameraMatrix.translation().x()) ;
    }
    cameraMatrix.translate(transSpeed * translation);

    // Update instance information
    if(autoRotate) {
        transformMatrix.rotate(1.0f, QVector3D(0.4f, 0.3f, 0.3f));
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

    gem->initializeBuffer(shaderProgram, &transformMatrix, false);

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


