#version 110

uniform sampler2D texture;
uniform float width;
uniform float height;

void main(void) {
    float x = gl_FragCoord.x/width;
    float y = gl_FragCoord.y/height;
    gl_FragColor = texture2D(texture, vec2(x, y));
}
