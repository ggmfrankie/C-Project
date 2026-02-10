
#version 460

in vec2 outTexCoord;
in vec3 mvVertexNormal;
in vec3 mvVertexPos;

out vec4 fragColor;

uniform sampler2D texture_sampler;
uniform int hasTexture;

uniform vec3 color;

uniform float transparency;
uniform float brightness;

void main() {
    if(hasTexture == 1){
        vec4 texColor = texture(texture_sampler, outTexCoord);
        fragColor = vec4(texColor.rgb * brightness, texColor.a * transparency);
    } else {
        fragColor = vec4(color * brightness, transparency);
    }
}