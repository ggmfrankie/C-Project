
#version 460

in vec2 outTexCoord;
in vec3 mvVertexNormal;
in vec3 mvVertexPos;

out vec4 fragColor;

uniform sampler2D texture_sampler;
uniform int hasTexture;
uniform int isActive;

void main() {

    if(hasTexture == 1){
        vec4 outFrag = texture(texture_sampler, outTexCoord);
        if (isActive == 1) {
            fragColor = texture(texture_sampler, outTexCoord);
        } else {
            discard;
        }
    } else {
        fragColor = vec4(0.0, 0.6, 1.0, 1.0);
    }


    //fragColor = vec4(0.0, 0.6, 1.0, 1.0);
}