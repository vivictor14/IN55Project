#version 330
out vec4 fColor;

in vec3 TexCoord;

uniform samplerCube skybox;

void main()
{
    // set the fragment texture
    fColor = texture(skybox, TexCoord);
}