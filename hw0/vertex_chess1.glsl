#version 110

uniform mat4 mvpMatrix;
attribute vec4 vert;
varying vec2 texcoord;

void main(void)
{
    gl_Position = mvpMatrix * vert;
    texcoord = gl_Position.xy * vec2(0.48) + vec2(0.67);
}
