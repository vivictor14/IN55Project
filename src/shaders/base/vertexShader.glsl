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

out vec3 TexCoords;

uniform mat4 modelToWorld;
uniform mat4 worldToCamera;
uniform mat4 cameraToView;
uniform ligth lumiere; // M

//
//void main()
//{
//
//    vec3 LightPosition_worldspace = vec3(1.0f, 1.0f, 0.0f);
//    // Output position of the vertex, in clip space : MVP * position
//    gl_Position =  cameraToView * worldToCamera * modelToWorld * vec4(position,1);
//
//    // Position of the vertex, in worldspace : M * position
//    vec3 Position_worldspace = (modelToWorld * vec4(position,1)).xyz;
//
//    // Vector that goes from the vertex to the camera, in camera space.
//    // In camera space, the camera is at the origin (0,0,0).
//    vec3 vertexPosition_cameraspace = ( worldToCamera * modelToWorld * vec4(position,1)).xyz;
//    vec3 EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;
//
//    // Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
//    vec3 LightPosition_cameraspace = ( worldToCamera * vec4(LightPosition_worldspace,1)).xyz;
//    LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;
//
//    // Normal of the the vertex, in camera space
//    vec3 Normal_cameraspace = ( worldToCamera * modelToWorld * vec4(6.0f, 6.0f, 6.0f,0)).xyz; // Only correct if ModelMatrix does not scale
//
//     vColor = vec4(color, 0.3);
//
//}

out vec4 amb;
out vec4 diff;
out vec4 spec;
out vec3 ColorT;

out vec3 Position;
out vec3 n;



void DirectionalLight(in vec3 V, in vec3 normal,inout vec4 ambient,inout vec4 diffuse, inout vec4 specular);
void PointLight(in vec3 V, in vec3 sp, in vec3 normal, inout vec4 ambient, inout vec4 diffuse, inout vec4 specular );

void main()
{

    mat3 glNormalMatrix = transpose(inverse(mat3(modelToWorld)));
    vec3 sp = vec3( worldToCamera * modelToWorld * vec4(position,1) );
    vec3 V = -normalize( sp );
    vec3 unit_normal = normalize( glNormalMatrix*normal.xyz );
    amb = vec4(0.0);  diff = vec4(0.0);  spec = vec4(0.0);

//    DirectionalLight(V, unit_normal, amb, diff, spec);
    PointLight(V, sp, unit_normal, amb, diff, spec);

    TexCoords = position;

    gl_Position =  cameraToView * worldToCamera * modelToWorld * vec4(position,1);

    ColorT = color;
    n = unit_normal;
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
        pf = pow(nDotH, 4*0.5);
    }
    ambient += lumiere.ambiant;
    diffuse += lumiere.diffuse * nDotLi;
    specular += lumiere.specular *pf;
}

void PointLight(in vec3 V, in vec3 sp, in vec3 normal, inout vec4 ambient, inout vec4 diffuse, inout vec4 specular )
{
    float nDotLi; // normal . light direction
    float nDotH; // normal . light half vector
    float pf; // power factor
    float attenuation; // computed attenuation factor
    float d;  // distance from surface to light source
    vec3 L;  // direction from surface to light position
    vec3 halfway_vector;  // direction of maximum highlights

    L = lumiere.pos.xyz - sp;
    d = length( L );
    L = normalize( L );
    attenuation = 1.0 / (0.4 +
    0.6 * d +
    0.1 * d * d);
    halfway_vector = normalize(L + V);
    nDotLi = max(0.0, dot(normal, L));
    nDotH = max(0.0, dot(normal, halfway_vector));

    if (nDotLi == 0.0)
        pf = 0.0;
    else
        pf = pow(nDotH, 42);

    ambient += lumiere.ambiant;
    diffuse += lumiere.diffuse * nDotLi;
    specular += lumiere.specular * pf;
}
