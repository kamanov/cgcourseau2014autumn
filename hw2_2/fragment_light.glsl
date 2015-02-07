#version 120

uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 specularColor;
uniform float ambientReflection;
uniform float diffuseReflection;
uniform float specularReflection;
uniform sampler2D texture;
uniform sampler2D normTexture;

varying vec3 varyingNormal;
varying vec3 varyingLightDirection;
varying vec3 varyingViewerDirection;
varying vec2 varyingTextureCoordinate;

void main(void)
{
    vec3 normal = normalize(texture2D(normTexture, varyingTextureCoordinate).rgb * 2.0 - 1.0);
    vec3 lightDirection = normalize(varyingLightDirection);
    vec3 viewerDirection = normalize(varyingViewerDirection);
    vec4 ambientIllumination = ambientReflection * ambientColor;
    vec4 diffuseIllumination = diffuseReflection * max(0.0, clamp(dot(lightDirection, normal), 0, 1)) * diffuseColor;
    vec4 specularIllumination = specularReflection * pow(max(0.0,dot(-reflect(lightDirection, normal), viewerDirection)), 100.0) * specularColor;
    gl_FragColor = texture2D(texture, varyingTextureCoordinate) * (ambientIllumination + diffuseIllumination) + specularIllumination;
}
