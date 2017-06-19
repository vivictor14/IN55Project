#version 330

in vec4 amb;
in vec4 diff;
in vec4 spec;
in vec3 Position;
in vec3 ColorT;
in float atte;

in vec3 Normale;

uniform samplerCube skybox;
uniform vec3 cameraPos;

out vec4 fColor;


void main()
    {
    vec3 ambient  = vec3(amb)  * vec3(ColorT); //couleur ambiante
    vec3 diffuse  = vec3(diff) * vec3(ColorT); //couleur diffuse
    vec3 specular = vec3(spec) * vec3(ColorT); // couleur spéculaire
    float ratio = 1.00 / 2.42; // ration passage de l'air au diamant

    vec3 I = normalize(Position - cameraPos);

    vec3 Refraction = refract(I, Normale, ratio);
    vec3 Reflection = reflect(I, Normale);

    fColor = vec4(ambient + diffuse*atte + specular*atte + texture(skybox, Reflection).rgb*atte+ texture(skybox, Refraction).rgb*atte, 1.0);

}

