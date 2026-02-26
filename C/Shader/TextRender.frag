#version 460

in vec2 outUV;
out vec4 fragColor;

uniform sampler2D fontAtlas;

uniform vec3 color;

void main() {
    float alpha = texture(fontAtlas, outUV).a;
    fragColor = vec4(color, alpha);
}
