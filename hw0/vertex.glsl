#version 110

uniform mat4 mvpMatrix;
attribute vec4 vert;

void main(void)
{
    gl_Position = mvpMatrix * vert;
}
