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

out vec3 Normal_cameraspace;
out vec3 LightDirection_cameraspace;
out vec4 vColor;

uniform mat4 modelToWorld; // P
uniform mat4 worldToCamera; // V
uniform mat4 cameraToView; // M
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

void DirectionalLight(in vec3 V, in vec3 normal,inout vec4 ambient,inout vec4 diffuse, inout vec4 specular);
void PointLight(in vec3 V, in vec3 sp, in vec3 normal, inout vec4 ambient, inout vec4 diffuse, inout vec4 specular );

void main()
{

    mat3 glNormalMatrix = transpose(inverse(mat3(modelToWorld)));
    vec3 sp = vec3( worldToCamera * modelToWorld * vec4(position,1) );
    vec3 V = -normalize( sp );
    vec3 unit_normal = normalize( glNormalMatrix*normal.xyz );
    vec4 amb = vec4(0.0);  vec4 diff = vec4(0.0);  vec4 spec = vec4(0.0);

//    DirectionalLight(vec3(cameraToView), unit_normal, amb, diff, spec);
    PointLight(V, sp, unit_normal, amb, diff, spec);

    vColor = vec4(color,1) * amb + vec4(color,1) * diff + vec4(color,1)  * spec;

    gl_Position =  cameraToView * worldToCamera * modelToWorld * vec4(position,1);
}



void DirectionalLight(in vec3 V, in vec3 normal,inout vec4 ambient,inout vec4 diffuse, inout vec4 specular)
{
    float nDotLi;  // normal . light direction
    float nDotH;   // normal . light half vector
    float pf;// power factor

    vec3 halfway_vector = normalize(V+vec3(0.f,0.f,0.f).xyz);
    nDotLi = max(0.0, dot(normal, normalize(vec3(0.f,10.f,0.f))));
    if (nDotLi == 0.0)
        pf = 0.0;
    else
    {
        nDotH = max(0.0, dot(normal, halfway_vector));
        pf = pow(nDotH, 4*1);
    }
    ambient += lumiere.ambiant;
    diffuse += lumiere.diffuse * nDotLi;
    specular += lumiere.specular ;
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
        pf = pow(nDotH, 0.6);

    ambient += lumiere.ambiant;
    diffuse += lumiere.diffuse * nDotLi;
    specular += lumiere.specular * pf;
}

//
//uniform mat4 modelToWorld; // M
//uniform mat4 worldToCamera; // V
//uniform mat4 cameraToView; // P
//
//layout(location = 0) in vec3 position;
//layout(location = 1) in vec3 color;
//layout(location = 2) in vec3 normal;
//
//out vec3 fragVert;
//out vec3 fragNormal;
//out mat4 model;
//out vec3 vColor;
//
//void main() {
//    // Pass some variables to the fragment shader
//    fragNormal = normalize(normal);
//    fragVert = normalize(position);
//
//    vColor = color;
//    model = modelToWorld;
//    // Apply all matrix transformations to vert
//    gl_Position = cameraToView * worldToCamera * modelToWorld * vec4(position, 1);
//}