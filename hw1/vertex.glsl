#version 110

uniform int rastr_mode;
uniform mat4 mvpMatrix;
attribute vec4 vert;
attribute vec4 norm;
varying vec4 color;
varying vec4 pos;

void main(void)
{
    gl_Position = mvpMatrix * vert;
    if (rastr_mode == 0) {
        color = vec4(0, 0, 1, 1);
    } else if (rastr_mode == 1){
        vec4 m_norm = mvpMatrix * norm;
        color = vec4(m_norm.y, m_norm.y, m_norm.y, 1);
    } else {
        pos = gl_Position;
    }
}
