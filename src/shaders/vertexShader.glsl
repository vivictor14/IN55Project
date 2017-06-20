#version 330

struct lumiere
{
  vec4 position;
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
};

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;

uniform mat4 modelToWorld;
uniform mat4 worldToCamera;
uniform mat4 cameraToView;
uniform lumiere light;

out vec4 amb; // ambient component
out vec4 diff; // diffuse component
out vec4 spec; // specular component
out vec3 ColorT;

out vec3 Position;  // position of the point in the correct space
out vec3 Normale; // normalized normal

out float attenuation; // attenuation factor

// method which calculate all the component
void PointLight(in vec3 V, in vec3 sp, in vec3 normal, inout vec4 ambient, inout vec4 diffuse, inout vec4 specular );

void main()
{

    // matrice permettant d'avoir une normale cohérente lors d'un changements de repère
    mat3 glNormalMatrix = transpose(inverse(mat3(modelToWorld)));

    vec3 sp = vec3(modelToWorld * vec4(position,1));
    vec3 V = -normalize( sp );
    vec3 unit_normal = normalize( glNormalMatrix*normal.xyz );

    // The ambient component have a default value because of the skybox which have a sun (and so a constant light)
    amb = vec4(0.2,0.2,0.2,1.0);  diff = vec4(0.0);  spec = vec4(0.0);

    PointLight(V, sp, unit_normal, amb, diff, spec);

    ColorT = color;
    Normale = unit_normal;
    Position = vec3(modelToWorld * vec4(position,1));

    gl_Position =  cameraToView * worldToCamera * modelToWorld * vec4(position,1);

}


void PointLight(in vec3 V, in vec3 sp, in vec3 normal, inout vec4 ambient, inout vec4 diffuse, inout vec4 specular )
{
    float nDotLi; // dot product between normal and light direction
    float nDotH; // dot product between normal and halfway_vector
    float pf; // power factor
    float d;  // distance between the surface and the light position
    vec3 L;  // direction between the surface and the light position
    vec3 halfway_vector;  // direction of maximum highlights

    L = light.position.xyz - sp;
    d = max(0.0,length( L ));
    L = normalize( L );
    halfway_vector = normalize(L + V);
    nDotLi = max(0.0, dot(normal, L));
    nDotH = max(0.0, dot(normal, halfway_vector));

    // 0.06 was found thanks to multiple test
    attenuation = 1.0 / (0.06*d);

    // light is perpendicular to the face, diffuse component is at his maximum
    if (nDotLi == 0.0)
        pf = 0.0;
    else
        pf = pow(nDotH, 0.88);

    ambient += light.ambient ;
    diffuse += light.diffuse * nDotLi ;
    specular += light.specular * pf;
}
