#version 110

uniform mat4 mvpMatrix;
uniform int rastr_mode;
uniform float vt;
uniform vec4 c;
uniform float k_per_max;
varying vec4 color;
varying vec4 pos;

void main(void) {
    if (rastr_mode == 2) {
        vec4 w_c = mvpMatrix * c;
        float t = abs(sin(vt + k_per_max * length(pos - w_c)));
        gl_FragColor = vec4(t, t, t, 1);
    } else {
        gl_FragColor = color;
    }
}
