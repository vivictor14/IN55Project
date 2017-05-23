#include <EncodeDecode.glsl>
#include <GlobalBuffer.ubo>

// Vertex Inputs
in vec3 vViewNormal;
in vec4 vCurrViewPosition;
in vec4 vCurrClipPosition;
in vec4 vPrevClipPosition;
flat in vec3 vDiffuse;
flat in vec4 vSpecular;

// Framebuffer Outputs
layout(location = 0) out vec4 fAmbient;
layout(location = 1) out vec4 fGeometry;
layout(location = 2) out vec4 fMaterial;
layout(location = 3) out vec4 fDynamics;

void main()
{
  //////////////////////////////////////////////////////////////////////////////
  // Deferred Buffer 1: Geometry Buffer
  // Encode Geometry information (Normal, Normal, Specular Exp., Depth)
  fGeometry.xy  = encodeNormal(normalize(vViewNormal));
  fGeometry.z   = encodeSpecularColorAverage(vSpecular.xyz);
  fGeometry.w   = encodeDepth(gl_FragCoord.z);

  //////////////////////////////////////////////////////////////////////////////
  // Deferred Buffer 2: Material Buffer
  // Encode Material information (Diffuse, Diffuse, Diffuse, Specular )
  fMaterial.xyz = vDiffuse;
  fMaterial.w   = encodeSpecularExponent(vSpecular.w);

  //////////////////////////////////////////////////////////////////////////////
  // Deferred Buffer 3: Dynamics Buffer
  // Encode Dynamics information (velocity, velocity, velocity, 0)
  vec2 currClipPos = (vCurrClipPosition.xy / vCurrClipPosition.w) * 0.5 + 0.5;
  vec2 prevClipPos = (vPrevClipPosition.xy / vPrevClipPosition.w) * 0.5 + 0.5;
  fDynamics.xy  = currClipPos - prevClipPos;

  //////////////////////////////////////////////////////////////////////////////
  // Perform the Ambient Pass
  // Get material and write ambient / atmospheric pass
  float linearDepth = (depthFar - length(vCurrCameraPosition)) / depthDiff;
  fAmbient = vec4(vDiffuse, 1.0) * ambientColor;
  fAmbient = fAmbient * linearDepth + (1.0 - linearDepth) * atmosphericColor;
}