#version 330
layout (location = 0) in vec3 Position;

out vec3 TexCoord;

uniform mat4 cameraToView;
uniform mat4 worldToCamera;

void main()
{
    // Coord of texture is the same as the position of the point in this case
    TexCoord = Position;
    gl_Position = cameraToView * mat4(mat3(worldToCamera)) * vec4(Position, 1.0);
}