#version 330

//in vec3 Normal_cameraspace;
//in vec3 LightDirection_cameraspace;
//in vec4 vColor;
//
//out vec4 fColor;
//
//uniform vec4 LightColor = {0.5f,0.5f,0.5f,0.5f};
//uniform float distance = 1;
//uniform float LightPower = 60;
//
//
//void main(){
//
//    // Normal of the computed fragment, in camera space
//    vec3 n = normalize( Normal_cameraspace );
//    // Direction of the light (from the fragment to the light)
//    vec3 l = normalize( LightDirection_cameraspace );
//
//    // Cosine of the angle between the normal and the light direction,
//    // clamped above 0
//    //  - light is at the vertical of the triangle -> 1
//    //  - light is perpendicular to the triangle -> 0
//    //  - light is behind the triangle -> 0
//    float cosTheta = clamp( dot( n,l ), 0,1 );
//
//    fColor = vColor;// * LightColor * LightPower * cosTheta / (distance*distance);
//
//}
//
//
//
//
in mat4 model;
in vec3 fragNormal;
in vec3 fragVert;
in vec3 vColor;

out vec4 fColor;

void main() {
    //calculate normal in world coordinates
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 normal = normalize(normalMatrix * fragNormal);

    //calculate the location of this fragment (pixel) in world coordinates
    vec3 fragPosition = vec3(model * vec4(fragVert, 1));

    //calculate the vector from this pixels surface to the light source
    vec3 surfaceToLight = vec3(0.0f,20.0f,20.0f) - fragPosition;

    //calculate the cosine of the angle of incidence
    float brightness = dot(normal, surfaceToLight) / (length(surfaceToLight) * length(normal));
    brightness = clamp(brightness, 0, 1);

    //calculate final color of the pixel, based on:
    // 1. The angle of incidence: brightness
    // 2. The color/intensities of the light: light.intensities
    // 3. The texture and texture coord: texture(tex, fragTexCoord)
    fColor = vec4(brightness * vec3(1.0f,1.0f,1.0f) * vColor, 1.0f);
}