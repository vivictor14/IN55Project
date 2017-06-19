#version 330

in vec4 amb; // Ambient component
in vec4 diff; // Diffuse component
in vec4 spec; // Specular component
in vec3 Position;
in vec3 ColorT;
in float attenuation; // attenuation factor (produce more shadow the further the light is)
in vec3 Normale; // face's normal

uniform samplerCube skybox; // skybox's texture
uniform vec3 cameraPos; // camera position in the correct space

out vec4 fColor;


void main()
    {
    vec3 ambient  = vec3(amb)  * vec3(ColorT); // ambient color
    vec3 diffuse  = vec3(diff) * vec3(ColorT); // diffuse color
    vec3 specular = vec3(spec) * vec3(ColorT); // specular color
    float ratio = 1.00 / 2.42; // ratio air/diamond

    vec3 I = normalize(Position - cameraPos); // direction of the vision's user

    vec3 Refraction = refract(I, Normale, ratio);
    vec3 Reflection = reflect(I, Normale);

    // The color is equal to the sum of all the intensity, the ambient is not affect by the attenuation because
    // it is not related to the light's distance
    fColor = vec4(ambient + diffuse*attenuation + specular*attenuation + texture(skybox, Reflection).rgb*attenuation+ texture(skybox, Refraction).rgb*attenuation, 1.0);

}

