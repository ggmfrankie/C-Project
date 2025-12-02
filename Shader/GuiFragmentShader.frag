
#version 460

in vec2 outTexCoord;
in vec3 mvVertexNormal;
in vec3 mvVertexPos;

out vec4 fragColor;

uniform sampler2D texture_sampler;
uniform int hasTexture;
uniform int state;
uniform vec3 color;

void main() {

    if(hasTexture == 1){
        vec4 texColor = texture(texture_sampler, outTexCoord);
        if(texColor.a == 0.0) discard;

        if (state == 0) {
            fragColor = texColor;
        } else
        if (state == 1){
            fragColor = vec4(0.5 * texColor.rgb, texColor.a);
        } else
        if (state == 2){
            fragColor = 0 * texColor;
        }

    } else {
        fragColor = vec4(color, 1.0);
    }

}