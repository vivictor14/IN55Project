#version 330

struct ligth
{
  vec4 pos;
  vec4 ambiant;
  vec4 diffuse;
  vec4 specular;
};

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;

uniform mat4 modelToWorld;
uniform mat4 worldToCamera;
uniform mat4 cameraToView;
uniform ligth lumiere;

out vec4 amb;
out vec4 diff;
out vec4 spec;
out vec3 ColorT;

out vec3 Position;
out vec3 Normale;

out float atte;


void DirectionalLight(in vec3 V, in vec3 normal,inout vec4 ambient,inout vec4 diffuse, inout vec4 specular);
void PointLight(in vec3 V, in vec3 sp, in vec3 normal, inout vec4 ambient, inout vec4 diffuse, inout vec4 specular );

void main()
{

    mat3 glNormalMatrix = transpose(inverse(mat3(modelToWorld)));
    vec3 sp = vec3( modelToWorld * vec4(position,1) );
    vec3 V = -normalize( sp );
    vec3 unit_normal = normalize( glNormalMatrix*normal.xyz );
    amb = vec4(0.0);  diff = vec4(0.0);  spec = vec4(0.0);

  //  DirectionalLight(V, unit_normal, amb, diff, spec);
    PointLight(V, sp, unit_normal, amb, diff, spec);

    gl_Position =  cameraToView * worldToCamera * modelToWorld * vec4(position,1);

    ColorT = color;
    Normale = unit_normal;
    Position = vec3(modelToWorld * vec4(position,1));
}



void DirectionalLight(in vec3 V, in vec3 normal,inout vec4 ambient,inout vec4 diffuse, inout vec4 specular)
{
    float nDotLi;  // normal . light direction
    float nDotH;   // normal . light half vector
    float pf;// power factor

    vec3 halfway_vector = normalize(V+lumiere.pos.xyz);
    nDotLi = max(0.0, dot(normal, normalize(lumiere.pos.xyz)));
    if (nDotLi == 0.0)
        pf = 0.0;
    else
    {
        nDotH = max(0.0, dot(normal, halfway_vector));
        pf = pow(nDotH, 4*0.6);
    }
    ambient += lumiere.ambiant;
    diffuse += lumiere.diffuse * nDotLi;
    specular += lumiere.specular *pf;
}

void PointLight(in vec3 V, in vec3 sp, in vec3 normal, inout vec4 ambient, inout vec4 diffuse, inout vec4 specular )
{
    float nDotLi; // produit scalaire entre normal et direction de la lumière
    float nDotH; // produit scalaire entre normal et le halfway_vector
    float pf; // power factor
    float attenuation;
    float d;  // la distance entre la surface et la position de la lumière
    vec3 L;  // la direction entre la surface et la position de la lumière
    vec3 halfway_vector;  // direction of maximum highlights

    L = lumiere.pos.xyz - sp;
    d = max(0.0,length( L ));
    L = normalize( L );
    halfway_vector = normalize(L + V);
    nDotLi = max(0.0, dot(normal, L));
    nDotH = max(0.0, dot(normal, halfway_vector));

    attenuation = 1.0 / (0.4 + 0.01 *d*d);

    if (nDotLi == 0.0)
        pf = 0.0;
    else
        pf = pow(nDotH, 0.6);

// valeur trouver pour un materiau de type émeraude
    ambient += lumiere.ambiant ;
    diffuse += lumiere.diffuse * nDotLi ;
    specular += lumiere.specular * pf;
    atte = attenuation;
}
