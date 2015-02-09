#version 110

uniform mat4 mvpMatrix;
uniform mat4 mvMatrix;
uniform mat3 normalMatrix;
uniform vec3 lightPosition;
attribute vec4 vert;
attribute vec3 norm;
attribute vec2 texcoord;
varying vec3 varyingNormal;
varying vec3 varyingLightDirection;
varying vec3 varyingViewerDirection;
varying vec2 varyingTextureCoordinate;

void main(void)
{
    vec4 eyeVertex = mvMatrix * vert;
    eyeVertex /= eyeVertex.w;
    varyingNormal = normalMatrix * norm;
    varyingLightDirection = lightPosition - eyeVertex.xyz;
    varyingViewerDirection = -eyeVertex.xyz;
    varyingTextureCoordinate = texcoord;
    gl_Position = mvpMatrix * vert;
}
