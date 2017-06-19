#include "gem.h"

/**
 * Parametrized constructor
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
Gem::Gem(GLfloat topHeight, GLfloat bottomHeight, GLfloat topRadius, GLfloat middleRadius, GLfloat bottomRadius,
         GLint topNbPoints, GLint middleNbPoints, GLint bottomNbPoints, GLint topComplexity, GLint bottomComplexity,
         GLint lengthStretchingPercent, GLint widthStretchingPercent, QColor color) {

    initGem(topHeight, bottomHeight, topRadius, middleRadius, bottomRadius, topNbPoints, middleNbPoints, bottomNbPoints,
            topComplexity, bottomComplexity, lengthStretchingPercent, widthStretchingPercent, color);

}

/**
 * Destructor
 *
 */
Gem::~Gem() {
    for(int i = 0; i < 4; i++) {
        vbo[i].destroy();
        vao[i].destroy();
    }
}

/**
 * Initialize the gem with the parameters
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
void Gem::initGem(GLfloat topHeight, GLfloat bottomHeight, GLfloat topRadius, GLfloat middleRadius,
                  GLfloat bottomRadius,
                  GLint topNbPoints, GLint middleNbPoints, GLint bottomNbPoints, GLint topComplexity,
                  GLint bottomComplexity,
                  GLint lengthStretchingPercent, GLint widthStretchingPercent, QColor color) {

    // Store the previous stretching values to cancel it later
    this->previousLengthStretchingPercent = this->lengthStretchingPercent;
    this->previousWidthStretchingPercent = this->widthStretchingPercent;

    this->topHeight = topHeight;
    this->bottomHeight = -bottomHeight; // Since the bottom part is upside down, the height has to be inverted
    this->topRadius = topRadius;
    this->middleRadius = middleRadius;
    this->bottomRadius = bottomRadius;
    this->topNbPoints = topNbPoints;
    this->middleNbPoints = middleNbPoints;
    this->bottomNbPoints = bottomNbPoints;
    this->topComplexity = topComplexity;
    this->bottomComplexity = bottomComplexity;
    this->lengthStretchingPercent = lengthStretchingPercent;
    this->widthStretchingPercent = widthStretchingPercent;
    this->color = color;

    initVertices(QVector3D(color.red() / 255.0f, color.green() / 255.0f, color.blue() / 255.0f));
    mapping();
}

/**
 * Initialize the vertices
 *
 * @param color A vector 3d representing the color
 */
void Gem::initVertices(QVector3D color) {
    middleVertices = new Vertex [ middleNbPoints ];

    // The angle between each middle vertex
    double angle = 2.0f * M_PI / middleNbPoints;
    for(int i = 0; i < middleNbPoints; i++) {
        float x = (float) cos(i * angle) * middleRadius;
        float z = (float) sin(i * angle) * middleRadius;
        middleVertices[i] = Vertex( QVector3D(x, 0, z), color);
    }

    // To avoid peaks, take the radius of the inner middle circle instead of the middle circle
    calculateInnerMiddleRadius();

    // Initialize vertices per face
    topVertices = initFaceVertices(true, topHeight, topRadius, topNbPoints, topComplexity, color);
    bottomVertices = initFaceVertices(false, bottomHeight, bottomRadius, bottomNbPoints, bottomComplexity, color);

}

/**
 * Calculate the radius of the inner middle circle
 *
 */
void Gem::calculateInnerMiddleRadius() {

    QVector3D A = middleVertices[0].position();
    QVector3D B = middleVertices[1].position();
    QVector3D M = QVector3D((A.x() + B.x()) / 2,
                            (A.y() + B.y()) / 2,
                            (A.z() + B.z()) / 2);
    innerMiddleRadius = (GLfloat) sqrt(pow(M.x(), 2) + pow(M.y(), 2) + pow(M.z(), 2));

}

/**
 *
 *
 * @param counterClockWise Whether the mapping should be reverted or not
 * @param height The height of the face
 * @param radius The radius of the extremity circle
 * @param nbPoints The number of points per circle
 * @param complexity The complexity
 * @param color The color
 * @return The face's vertices
 */
Vertex *Gem::initFaceVertices(bool counterClockWise, GLfloat height, GLfloat radius, GLint nbPoints, GLint complexity, QVector3D color) {

    Vertex *vertices;

    // If the number of points is 1, then the generation should use the middle number of points and have a culet
    int culet = 0;
    if(nbPoints == 1) {
        radius = 0;
        nbPoints = middleNbPoints;
        culet = 1;
    }

    vertices = new Vertex[ (complexity - culet) * nbPoints + culet ];

    // The angle between each face vertex
    double angle = 2.0f * M_PI / nbPoints;

    // The offset to apply in order to have a shift between the circles
    double offset;

    for (int i = 0; i < complexity - culet; i++) {

        offset = (i + 1) * angle / 2;

        // The radius of the interpolated circle
        float complexityCircleRadius = innerMiddleRadius - (innerMiddleRadius - radius) * (i + 1) / (float)complexity;

        for (int j = 0; j < nbPoints; j++) {

            float x, y, z;

            x = (float) cos(j * angle + offset) * complexityCircleRadius;
            z = (float) sin((j * angle + offset) * (2 * counterClockWise - 1)) * complexityCircleRadius;
            y = (i + 1) * (height / complexity);
            vertices[(i * nbPoints) + j] = Vertex(QVector3D(x, y, z), color);

        }
    }

    // Add a culet if needed
    if(culet) {
        vertices[(complexity - culet) * nbPoints] = Vertex(QVector3D(0, height, 0), color);
    }

    return vertices;

}

/**
 * Map the faces
 *
 */
void Gem::mapping() {

    mappings = new VerticesMapping[8];

    faceMapping(0, bottomVertices, bottomNbPoints, bottomComplexity, true);
    faceMapping(4, topVertices, topNbPoints, topComplexity, false);

}

/**
 * Map a face
 *
 * @param startPosition The index to start with in the mappings array
 * @param vertices The face vertices
 * @param nbPoints The number of points per circle
 * @param complexity The complexity of the face
 * @param clockwise Whether the mapping should be reverted or not
 */
void Gem::faceMapping(int startPosition, Vertex *vertices, GLint nbPoints, GLint complexity,
                      bool clockwise) {

    // If more than a point, use a table, else use a pavilion
    if(nbPoints > 1) {
        tableMapping(&mappings[startPosition], vertices, nbPoints, complexity);
        // There's a star only if the complexity is greater than 1
        if(complexity > 1) {
            starMapping(&mappings[startPosition + 1], vertices, nbPoints, complexity);

            // There are bezels only if the complexity is greater than 2
            if(complexity > 2) {
                bezelMapping(&mappings[startPosition + 2], vertices, nbPoints, complexity);
            }
        }
        upperGirdleMapping(&mappings[startPosition + 3], vertices, nbPoints, complexity, clockwise);
    }
    else {
        pavilionMapping(&mappings[startPosition], vertices, complexity, clockwise);

        // There's a upper girdle only if the complexity is greater than 1
        if(complexity > 1) {

            // There are a star and bezels only if the complexity is greater than 2
            if (complexity > 2) {
                starMapping(&mappings[startPosition + 1], vertices, middleNbPoints, complexity - 1);
                bezelMapping(&mappings[startPosition + 2], vertices, middleNbPoints, complexity - 1);
            }
            upperGirdleMapping(&mappings[startPosition + 3], vertices, middleNbPoints, complexity - 1, clockwise);
        }
    }

}

/**
 * Map a table
 *
 * @param mapping The table mapping
 * @param vertices The face vertices
 * @param nbPoints The number of points per circle
 * @param complexity The complexity of the face
 */
void Gem::tableMapping(VerticesMapping *mapping, Vertex *vertices, GLint nbPoints, GLint complexity) {

    mapping->mode = GL_TRIANGLES;
    mapping->length = nbPoints * 3 ;
    mapping->vertices = new Vertex[mapping->length];

    // Map all the vertices of the topmost circle to obtain the table
    for(int i = 0; i < nbPoints; i++) {

        mapping->vertices[3 * i] = vertices[complexity * nbPoints - 1];
        mapping->vertices[3 * i + 1] = vertices[(complexity * nbPoints - 1) - (i + 1) % nbPoints];
        mapping->vertices[3 * i + 2] = vertices[(complexity * nbPoints - 1) - (i + 2) % nbPoints];

    }
}

/**
 * Map a pavilion
 *
 * @param mapping The table mapping
 * @param vertices The face vertices
 * @param complexity The complexity of the face
 * @param clockwise Whether the mapping should be reverted or not
 */
void Gem::pavilionMapping(VerticesMapping *mapping, Vertex *vertices, GLint complexity, bool clockWise) {

    mapping->mode = GL_TRIANGLES;
    mapping->length  = (middleNbPoints + 2) * 3;
    mapping->vertices = new Vertex[mapping->length];

    if(complexity > 1) {

        // Map all the vertices of the topmost complexity circle to the culet
        for (int i = 0; i <= middleNbPoints; i++) {

            mapping->vertices[3 * i] = vertices[middleNbPoints * (complexity - 1)];
            mapping->vertices[3 * i + 1] = vertices[(complexity - 1) * middleNbPoints - 1 - i % middleNbPoints];
            mapping->vertices[3 * i + 2] = vertices[(complexity - 1) * middleNbPoints - 1 - (i + 1) % middleNbPoints];

        }
    }
    else {

        // Map all the vertices of the middle circle to the culet
        for (int i = 0; i <= middleNbPoints; i++) {
            if(clockWise) {
                mapping->vertices[3 * i] = vertices[middleNbPoints * (complexity - 1)];
                mapping->vertices[3 * i + 1] = middleVertices[i % middleNbPoints];
                mapping->vertices[3 * i + 2] = middleVertices[(i + 1) % middleNbPoints];
            }
            else {
                mapping->vertices[3 * i] = vertices[middleNbPoints * (complexity - 1)];
                mapping->vertices[3 * i + 1] = middleVertices[(middleNbPoints - 1) - i % middleNbPoints];
                mapping->vertices[3 * i + 2] = middleVertices[(middleNbPoints - 1) - (i + 1) % middleNbPoints];
            }
        }
    }

}

/**
 * Map a star
 *
 * @param mapping The table mapping
 * @param vertices The face vertices
 * @param nbPoints The number of points per circle
 * @param complexity The complexity of the face
 */
void Gem::starMapping(VerticesMapping *mapping, Vertex *vertices, GLint nbPoints, GLint complexity) {

    mapping->mode = GL_TRIANGLES;
    mapping->length = 3 * nbPoints;
    mapping->vertices = new Vertex[mapping->length];

    // Map the vertices of the topmost circle to the vertices of the topmost complexity circle
    for(int i = 0; i < nbPoints; i++) {

        mapping->vertices[3 * i] = vertices[nbPoints * (complexity - 1) + i];
        mapping->vertices[3 * i + 1] = vertices[nbPoints * (complexity - 1) + (i + 1) % nbPoints];
        mapping->vertices[3 * i + 2] = vertices[nbPoints * (complexity - 2) + (i + 1) % nbPoints];

    }

}

/**
 * Map bezels
 *
 * @param mapping The table mapping
 * @param vertices The face vertices
 * @param nbPoints The number of points per circle
 * @param complexity The complexity of the face
 */
void Gem::bezelMapping(VerticesMapping *mapping, Vertex *vertices, GLint nbPoints, GLint complexity) {

    mapping->mode = GL_TRIANGLES;
    mapping->length = 6 * nbPoints * (complexity - 2);
    mapping->vertices = new Vertex[mapping->length];

    // Map lozenges between the star and the upper girdle
    for(int i = 0; i < complexity - 2; i++) {

        for(int j = 0; j < nbPoints; j++) {

            mapping->vertices[6 * (nbPoints * i + j)] = vertices[i * nbPoints + j];
            mapping->vertices[6 * (nbPoints * i + j) + 1] = vertices[(i + 1) * nbPoints + (nbPoints - 1 + j) % nbPoints];
            mapping->vertices[6 * (nbPoints * i + j) + 2] = vertices[(i + 2) * nbPoints + (nbPoints - 1 + j) % nbPoints];
            mapping->vertices[6 * (nbPoints * i + j) + 3] = vertices[(i + 2) * nbPoints + (nbPoints - 1 + j) % nbPoints];
            mapping->vertices[6 * (nbPoints * i + j) + 4] = vertices[(i + 1) * nbPoints + j];
            mapping->vertices[6 * (nbPoints * i + j) + 5] = vertices[i * nbPoints + j];

        }
    }


}

/**
 * Map an upper girdle
 *
 * @param mapping The table mapping
 * @param vertices The face vertices
 * @param nbPoints The number of points per circle
 * @param complexity The complexity of the face
 * @param clockwise Whether the mapping should be reverted or not
 */
void Gem::upperGirdleMapping(VerticesMapping *mapping, Vertex *vertices, GLint nbPoints, GLint complexity, bool clockWise) {

    mapping->mode = GL_TRIANGLES;
    mapping->length = (2 + (complexity > 1)) * 3 * nbPoints + 6 * middleNbPoints;
    mapping->vertices = new Vertex[mapping->length];
    int startIndex = 0;

    // Map a triangle for every point on the middle circle and for every point on the first complexity circle

    for(int i = 0; i < nbPoints; i++) {

        if(clockWise) {

            mapping->vertices[3 * i] = vertices[nbPoints - 1 - i];
            mapping->vertices[3 * i + 1] = middleVertices[(middleNbPoints - 1 + (int) round((i + 1) * middleNbPoints / (float) nbPoints)) % middleNbPoints];
            mapping->vertices[3 * i + 2] = middleVertices[(int) round((i + 1) * middleNbPoints / (float) nbPoints) % middleNbPoints];

        }
        else {

            mapping->vertices[3 * i] = vertices[i];
            mapping->vertices[3 * i + 1] = middleVertices[(int) round((i + 1) * middleNbPoints / (float) nbPoints) % middleNbPoints];
            mapping->vertices[3 * i + 2] = middleVertices[(middleNbPoints - 1 + (int) round((i + 1) * middleNbPoints / (float) nbPoints)) % middleNbPoints];

        }

    }

    startIndex = 3 * nbPoints;

    if(complexity == 1) {
        for (int i = 0; i < middleNbPoints; i++) {

            if (clockWise) {

                mapping->vertices[startIndex + 3 * i] = middleVertices[middleNbPoints - 1 - i];
                mapping->vertices[startIndex + 3 * i + 1] = vertices[(int) round((i + 1) * nbPoints / (float) middleNbPoints) - 1];
                mapping->vertices[startIndex + 3 * i + 2] = vertices[(int) round((i + 1) * nbPoints / (float) middleNbPoints) % nbPoints];

            } else {

                mapping->vertices[startIndex + 3 * i] = middleVertices[(i + 1) % middleNbPoints];
                mapping->vertices[startIndex + 3 * i + 2] = vertices[(int) round((i + 1) * nbPoints / (float) middleNbPoints) % nbPoints];
                mapping->vertices[startIndex + 3 * i + 1] = vertices[(int) round((i + 1) * nbPoints / (float) middleNbPoints) - 1];

            }

        }
    }

    startIndex = 3 * nbPoints + 3 * middleNbPoints;

    for(int i = 0; i < middleNbPoints; i++) {

        if(clockWise){

            mapping->vertices[startIndex + 3 * i] = middleVertices[i];
            mapping->vertices[startIndex + 3 * i + 1] = middleVertices[(i + 1) % middleNbPoints];
            mapping->vertices[startIndex + 3 * i + 2] = vertices[nbPoints - 1 - (int)round(i * nbPoints / (float)middleNbPoints) % nbPoints];

        }
        else {

            mapping->vertices[startIndex + 3 * i] = middleVertices[(i + 1) % middleNbPoints];
            mapping->vertices[startIndex + 3 * i + 1] = middleVertices[i];
            mapping->vertices[startIndex + 3 * i + 2] = vertices[(int)round(i * nbPoints / (float)middleNbPoints) % nbPoints];

        }

    }

    startIndex = 3 * nbPoints + 6 * middleNbPoints;

    if(complexity == 1) {
        for (int i = 0; i < nbPoints; i++) {

            if(clockWise) {

                mapping->vertices[startIndex + 3 * i] = vertices[nbPoints - 1 - i];
                mapping->vertices[startIndex + 3 * i + 1] = vertices[(nbPoints - i) % nbPoints];
                mapping->vertices[startIndex + 3 * i + 2] = middleVertices[((int)round(i * middleNbPoints / (float)nbPoints)) % middleNbPoints];

            }
            else {

                mapping->vertices[startIndex + 3 * i] = vertices[(nbPoints + i - 1) % nbPoints ];
                mapping->vertices[startIndex + 3 * i + 1] = vertices[i];
                mapping->vertices[startIndex + 3 * i + 2] = middleVertices[((int)round(i * middleNbPoints / (float)nbPoints)) % middleNbPoints];

            }

        }
    }
    else {

        // Map a lozenge for every point on the first complexity circle

        for (int i = 0; i < nbPoints; i++) {

            if(clockWise) {

                mapping->vertices[startIndex + 6 * i] = vertices[nbPoints + (nbPoints - i - 1) % nbPoints];
                mapping->vertices[startIndex + 6 * i + 1] = vertices[(nbPoints - i) % nbPoints];
                mapping->vertices[startIndex + 6 * i + 2] = middleVertices[(int)round(i * middleNbPoints / (float)nbPoints) % middleNbPoints];
                mapping->vertices[startIndex + 6 * i + 3] = middleVertices[(int)round(i * middleNbPoints / (float)nbPoints) % middleNbPoints];
                mapping->vertices[startIndex + 6 * i + 4] = vertices[nbPoints - 1 - i];
                mapping->vertices[startIndex + 6 * i + 5] = vertices[nbPoints + (nbPoints - i - 1) % nbPoints];

            }
            else {

                mapping->vertices[startIndex + 6 * i] = vertices[nbPoints + (nbPoints + i - 1) % nbPoints];
                mapping->vertices[startIndex + 6 * i + 1] = vertices[i];
                mapping->vertices[startIndex + 6 * i + 2] = middleVertices[(int)round(i * middleNbPoints / (float)nbPoints) % middleNbPoints];
                mapping->vertices[startIndex + 6 * i + 3] = middleVertices[(int)round(i * middleNbPoints / (float)nbPoints) % middleNbPoints];
                mapping->vertices[startIndex + 6 * i + 4] = vertices[(nbPoints + i - 1) % nbPoints];
                mapping->vertices[startIndex + 6 * i + 5] = vertices[nbPoints + (nbPoints + i - 1) % nbPoints];

            }
        }
    }
}

/**
 * Initialize the openGL context for the gem
 *
 * @param shaderProgram The shader program to use
 * @param transformMatrix The transformation matrix
 * @param init Whether this function is called in the initialization phase or not
 */
void Gem::initializeBuffer(QOpenGLShaderProgram *shaderProgram, Transform3D *transformMatrix, bool init) {

    initializeOpenGLFunctions();

    if(init) {
        shaderProgram->bind();
        vbo = new QOpenGLBuffer[8];
        vao = new QOpenGLVertexArrayObject[8];
    }

    // Use the stretching percents and the last stretching percents to correctly scale the gem
    transformMatrix->scale(lengthStretchingPercent / (float)previousLengthStretchingPercent, 1,
                       widthStretchingPercent / (float)previousWidthStretchingPercent);

    // Populate the buffer and the vao for every part of the gem
    for(int i = 0; i < 8; i++) {

        if(mappings[i].length > 0) {

            calculateNormal(&mappings[i]);

            if(init) {
                vbo[i].create();
            }

            vbo[i].bind();
            vbo[i].setUsagePattern(QOpenGLBuffer::StaticDraw);
            vbo[i].allocate(mappings[i].vertices, mappings[i].length * sizeof(Vertex));

            if(init) {
                vao[i].create();
            }

            vao[i].bind();
            shaderProgram->enableAttributeArray(0);
            shaderProgram->enableAttributeArray(1);
            shaderProgram->enableAttributeArray(2);
            shaderProgram->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
            shaderProgram->setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());
            shaderProgram->setAttributeBuffer(2, GL_FLOAT, Vertex::normalOffset(), Vertex::NormalTupleSize, Vertex::stride());

            vao[i].release();
            vbo[i].release();
        }
    }

    if(init) {
        shaderProgram->release();
    }

}

/**
 * Draw the gem
 *
 * @param shaderProgram The shader program to use
 * @param modelToWorldLocation The location for the modelToWorld matrix
 * @param transformMatrix The transformation matrix
 * @param cubeMapTexture The texture to use
 */
void Gem::drawShape(QOpenGLShaderProgram *shaderProgram, int modelToWorldLocation, Transform3D transformMatrix, unsigned int cubeMapTexture) {

    for(int i = 0; i < 8; i++) {
        if(mappings[i].length > 0) {
            vao[i].bind();
            shaderProgram->setUniformValue(modelToWorldLocation, transformMatrix.toMatrix());
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
            glDepthMask(GL_TRUE);
            glDrawArrays(mappings[i].mode, 0, mappings[i].length);
            vao[i].release();
        }
    }

}

/**
 * Calculate the normal per face
 *
 * @param Mapping
 */
void Gem::calculateNormal(VerticesMapping *Mapping) {

    Vertex point1, point2, point3;
    float x, y, z;
    QVector3D *u, *v, *n;

    for(int i = 0; i < Mapping->length; i = i + 3) {

        point1 = Mapping->vertices[i];
        point2 = Mapping->vertices[i + 1];
        point3 = Mapping->vertices[i + 2];
        u = new QVector3D(point2.position() - point1.position());
        v = new QVector3D(point3.position() - point1.position());
        x = (u->y() * v->z() - (u->z() * v->y()));
        y = (u->z() * v->x() - (u->x() * v->z()));
        z = (u->x() * v->y() - (u->y() * v->x()));
        n = new QVector3D(x, y, z);
        Mapping->vertices[i].setNormal(*n);
        Mapping->vertices[i + 1].setNormal(*n);
        Mapping->vertices[i + 2].setNormal(*n);

    }
}

/**
 * Get the top height
 *
 * @return The top height
 */
GLfloat Gem::getTopHeight() const {
    return topHeight;
}

/**
 * Get the bottom height
 *
 * @return The bottom height
 */
GLfloat Gem::getBottomHeight() const {
    return -bottomHeight;
}

/**
 * Get the top radius
 *
 * @return The top radius
 */
GLfloat Gem::getTopRadius() const {
    return topRadius;
}

/**
 * Get the middle radius
 *
 * @return The middle radius
 */
GLfloat Gem::getMiddleRadius() const {
    return middleRadius;
}

/**
 * Get the bottom radius
 *
 * @return The bottom radius
 */
GLfloat Gem::getBottomRadius() const {
    return bottomRadius;
}

/**
 * Get the top number of points
 *
 * @return The top number of points
 */
GLint Gem::getTopNbPoints() const {
    return topNbPoints;
}

/**
 * Get the middle number of points
 *
 * @return The middle number of points
 */
GLint Gem::getMiddleNbPoints() const {
    return middleNbPoints;
}

/**
 * Get the bottom number of points
 *
 * @return The bottom number of points
 */
GLint Gem::getBottomNbPoints() const {
    return bottomNbPoints;
}

/**
 * Get the top complexity
 *
 * @return The top complexity
 */
GLint Gem::getTopComplexity() const {
    return topComplexity;
}

/**
 * Get the bottom complexity
 *
 * @return The bottom complexity
 */
GLint Gem::getBottomComplexity() const {
    return bottomComplexity;
}

/**
 * Get the color
 *
 * @return The color
 */
const QColor &Gem::getColor() const {
    return color;
}

/**
 * Get the length stretching percent
 *
 * @return The length stretching percent
 */
GLint Gem::getLengthStretchingPercent() const {
    return lengthStretchingPercent;
}

/**
 * Get the width stretching percent
 *
 * @return The width stretching percent
 */
GLint Gem::getWidthStretchingPercent() const {
    return widthStretchingPercent;
}
