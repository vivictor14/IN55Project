//
// Created by Anthony on 09/06/2017.
//
#include "SkyBox.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SkyBox::SkyBox() {

    mapping = new VerticesMapping[1];

    mapping[0].mode = GL_TRIANGLES;
    mapping[0].length = 12 * 3 ;
    mapping[0].vertices = new Vertex[mapping->length];

    static const GLfloat vertices[] = {
            // positions
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
    };

    for(int i=0;i<mapping[0].length;i++){

        mapping[0].vertices[i] = Vertex(QVector3D(vertices[3*i],vertices[3*i+1],vertices[3*i+2]));

    }

}

SkyBox::~SkyBox() {

    vbo[0].destroy();
    vao[0].destroy();

}

void SkyBox::initializeBuffer(QOpenGLShaderProgram *shaderProgram) {

    initializeOpenGLFunctions();

    std::vector<std::string> faces =
            {
                    "..\\resources\\Texture\\leftSkybox.jpg",
                    "..\\resources\\Texture\\rightSkybox.jpg",
                    "..\\resources\\Texture\\topSkybox.jpg",
                    "..\\resources\\Texture\\bottomSkybox.jpg",
                    "..\\resources\\Texture\\backSkybox.jpg",
                    "..\\resources\\Texture\\frontSkybox.jpg"
            };

    cubemapTexture = loadCubemap(faces);


    shaderProgram->bind();
    vbo = new QOpenGLBuffer[1];
    vao = new QOpenGLVertexArrayObject[1];

    vbo[0].create();

    vbo[0].bind();
    vbo[0].setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo[0].allocate(mapping[0].vertices, mapping[0].length * sizeof(Vertex));

    vao[0].create();


    vao[0].bind();
    shaderProgram->enableAttributeArray(0);
    shaderProgram->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize,
                                      Vertex::stride());

    vao[0].release();
    vbo[0].release();


    shaderProgram->release();

}

void SkyBox::drawShape() {

    vao[0].bind();
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDepthMask(GL_TRUE);
    glDrawArrays(mapping[0].mode, 0, mapping[0].length);
    vao[0].release();

}

unsigned int SkyBox::getTexture(){
    return cubemapTexture;
}


unsigned int SkyBox::loadCubemap(std::vector<std::string> faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    int width, height, nrChannels;
    for (unsigned int i = 0; i < 6; i++) {

        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        } else {
            qDebug() << "Cubemap texture failed to load at path: " << faces[i].c_str();
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}