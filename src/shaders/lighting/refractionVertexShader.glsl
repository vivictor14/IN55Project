#version 120

// le ration pour la couleur se calcule avec F = f + (1 - f) (1 - V • N)^5

const float Eta = 1/2.4;         // Indice de refraction n1/n2 ici air sur diamant

const float F  = ((1.0-Eta) * (1.0-Eta)) / ((1.0+Eta) * (1.0+Eta));

varying vec3  Reflect;
varying vec3  Refract;
varying float Ratio;

void main()
{

    vec4 ecPosition  = gl_ModelViewMatrix * gl_Vertex;
    vec3 ecPosition3 = ecPosition.xyz / ecPosition.w;

    vec3 i = normalize(ecPosition3);
    vec3 n = normalize(gl_NormalMatrix * gl_Normal); //récupère la normal du vertex

    Ratio   = F + (1.0 - F) * pow((1.0 - dot(-i, n)), 5); // Calcul le ration pour le fragmentShader

    Refract = refract(i, n, Eta); // Voir fonction opengl
    Refract = vec3(gl_TextureMatrix[0] * vec4(Refract, 1.0));

    Reflect = reflect(i, n);
    Reflect = vec3(gl_TextureMatrix[0] * vec4(Reflect, 1.0));

    gl_Position = ftransform();
}