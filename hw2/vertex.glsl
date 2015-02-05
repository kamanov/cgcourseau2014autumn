#version 110

uniform mat4 mvpMatrix;
attribute vec4 vert;
attribute vec2 texcoord;

varying vec2 out_tex_coord;
uniform float k;


void main(void)
{
    gl_Position = mvpMatrix * vert;
    out_tex_coord = k * texcoord;
}
