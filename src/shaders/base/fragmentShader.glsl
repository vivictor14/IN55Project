#version 330

in vec3 TexCoords;

out vec4 fColor;

in vec4 amb;
in vec4 diff;
in vec4 spec;
in vec3 Position;
in vec3 ColorT;

in vec3 n;

uniform samplerCube skybox;
uniform vec3 cameraPos;

void main()
{
vec3 ambient  = vec3(amb)  * vec3(ColorT);
vec3 diffuse  = vec3(diff) * vec3(ColorT);
vec3 specular = vec3(spec) * vec3(ColorT);
float ratio = 1.00 / 2.42;

    vec3 I = normalize(Position - cameraPos);

    vec3 Refraction = refract(I, n, ratio);
    vec3 Reflection = reflect(I, n);

    fColor = vec4(ambient + diffuse + specular + texture(skybox, Reflection).rgb + texture(skybox, Refraction).rgb, 1.0);

       // fColor =  vec4(color,1); //* vec4(texture(skybox, Refract).rgb, 1.0) *  vec4(texture(skybox, Reflect).rgb, 1.0);
}

