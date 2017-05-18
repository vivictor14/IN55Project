#include "displayWindow.h"

static const Vertex sg_vertexes[] = {
        Vertex( QVector3D( 0.00f,  0.75f, 1.0f), QVector3D(1.0f, 0.0f, 0.0f) ),
        Vertex( QVector3D( 0.75f, -0.75f, 1.0f), QVector3D(0.0f, 1.0f, 0.0f) ),
        Vertex( QVector3D(-0.75f, -0.75f, 1.0f), QVector3D(0.0f, 0.0f, 1.0f) )
};

DisplayWindow::DisplayWindow() {

}

DisplayWindow::~DisplayWindow() {

}

void DisplayWindow::keyPressEvent(QKeyEvent *event) {

}

void DisplayWindow::initializeGL() {
    initializeOpenGLFunctions();

    // Set global information
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Application-specific initialization
    {
        // Create Shader (Do not release until VAO is created)
        shaderProgram = new QOpenGLShaderProgram();
        shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, "C:\\Users\\victo\\OneDrive\\Documents\\CLionProjects\\IN55Project\\src\\shaders\\vertexShader.glsl");
        shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, "C:\\Users\\victo\\OneDrive\\Documents\\CLionProjects\\IN55Project\\src\\shaders\\fragmentShader.glsl");
        shaderProgram->link();
        shaderProgram->bind();

        // Create Buffer (Do not release until VAO is created)
        buffer.create();
        buffer.bind();
        buffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
        buffer.allocate(sg_vertexes, sizeof(sg_vertexes));

        // Create Vertex Array Object
        vao.create();
        vao.bind();
        shaderProgram->enableAttributeArray(0);
        shaderProgram->enableAttributeArray(1);
        shaderProgram->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
        shaderProgram->setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());

        // Release (unbind) all
        vao.release();
        buffer.release();
        shaderProgram->release();
    }
}

void DisplayWindow::paintGL() {
    // Clear
    glClear(GL_COLOR_BUFFER_BIT);

    // Render using our shader
    shaderProgram->bind();
    {
        vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, sizeof(sg_vertexes) / sizeof(sg_vertexes[0]));
        vao.release();
    }
    shaderProgram->release();
}

void DisplayWindow::teardownGL() {
    vao.destroy();
    buffer.destroy();
    delete shaderProgram;
}
