#include <GBuffer.glsl>
#include <Math.glsl> // saturate

// Light Attributes
flat in vec4 vLightAttenuation;
flat in vec3 vLightViewPosition;
flat in vec3 vLightDiffuse;
flat in vec3 vLightSpecular;

// Fragment Position
layout(location = 0) out vec4 fFragColor;

void main()
{
  // GBuffer Access
  vec3 viewPos = viewPosition();

  vec3  lightVec   = vLightViewPosition - viewPos;
  float lightDist  = length(lightVec);
  if (lightDist < vLightAttenuation.w)
  {
    vec3 normal   = normal();
    vec3 diffuse  = diffuse();
    vec4 specular = specular();

    // Construct a finite attenuation
    vec3  lightDir    = lightVec / lightDist;
    vec3  polynomial  = vec3(1.0, lightDist, lightDist * lightDist);
    float attenuation = 1.0 / dot(polynomial,vLightAttenuation.xyz);
    attenuation *= saturate(1.0 - (lightDist / vLightAttenuation.w));

    // Blinn Phong
    float lambertian = max(dot(lightDir, normal), 0.0);
    float specFactor = 0.0;
    if (lambertian > 0.0)
    {
      vec3  viewDir    = normalize(-viewPos);
      vec3  halfDir    = normalize(lightDir + viewDir);
      float specAngle  = max(dot(halfDir, normal), 0.0);
      specFactor = pow(specAngle, specular.w);
    }

    // Construct Lighting Terms
    vec3 diffuseTerm  = vLightDiffuse  * diffuse      * lambertian;
    vec3 specularTerm = vLightSpecular * specular.xyz * specFactor;
    fFragColor = vec4(attenuation * (diffuseTerm + specularTerm), 1.0);
    fFragColor+= vec4(1.0, 0.0, 0.0, 1.0);
  }
  else
  {
    fFragColor = vec4(1.0);
  }
}