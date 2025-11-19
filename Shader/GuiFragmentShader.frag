
#version 460

in vec2 outTexCoord;
in vec3 mvVertexNormal;
in vec3 mvVertexPos;

out vec4 fragColor;

uniform sampler2D texture_sampler;
uniform int hasTexture;
uniform int state;

void main() {
    vec4 texColor = texture(texture_sampler, outTexCoord);
    if(texColor.a == 0.0) discard;
    if(hasTexture == 1){

        if (state == 0) {
            fragColor = texColor;
        } else
        if (state == 1){
            fragColor = 0.5 * texColor;
        } else
        if (state == 2){
            fragColor = 0 * texColor;
        }

    } else {
        fragColor = vec4(0.0, 0.6, 1.0, 1.0);
    }

}