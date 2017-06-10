#version 330
out vec4 fColor;

in vec3 TexCoord;

uniform samplerCube skybox;

void main()
{
    fColor = texture(skybox, TexCoord);
}