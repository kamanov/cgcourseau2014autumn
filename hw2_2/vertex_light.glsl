#version 120

uniform mat4 mvpMatrix;
uniform mat4 mvMatrix;
uniform mat3 normalMatrix;
uniform vec3 lightPosition;
attribute vec4 vert;
attribute vec3 norm;
attribute vec2 texcoord;
attribute vec3 tangents;
attribute vec3 bitangents;

varying vec3 varyingLightDirection;
varying vec3 varyingViewerDirection;
varying vec2 varyingTextureCoordinate;

void main(void)
{
    vec4 eyeVertex = mvMatrix * vert;
    eyeVertex /= eyeVertex.w;
    vec3 mv_normal = normalMatrix * normalize(norm);
    vec3 mv_tangent = normalMatrix * normalize(tangents);
    vec3 mv_bitangent = normalMatrix * normalize(bitangents);
    mat3 tbn = transpose(mat3(mv_tangent, mv_bitangent, mv_normal));

    varyingLightDirection = tbn * (lightPosition - eyeVertex.xyz);
    varyingViewerDirection = tbn * (-eyeVertex.xyz);
    varyingTextureCoordinate = texcoord;
    gl_Position = mvpMatrix * vert;
}
