#version 440

in vec2 outUV;
out vec4 FragColor;

uniform sampler2D fontAtlas;

uniform vec3 color;

void main() {
    vec4 textColor = texture(fontAtlas, outUV);
    textColor.rgb = color;
    FragColor = textColor;
}
