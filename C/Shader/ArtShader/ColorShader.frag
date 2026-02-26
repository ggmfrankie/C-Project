#version 440

in vec2 outUV;
out vec4 FragColor;

uniform vec3 color;

vec4 mainImage(in vec2 fragCoord);

void main() {
    FragColor = mainImage(outUV);
}

vec4 mainImage(in vec2 fragCoord){
    return vec4(0.0);
}
