#version 110

uniform sampler2D texture;
varying vec2 out_tex_coord;

void main(void) {
    gl_FragColor = texture2D(texture, out_tex_coord);
}
