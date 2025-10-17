#version 460

in vec3 outPosition;

out vec4 fragColor;

uniform sampler2D textureSampler;
uniform int hasTexture;
uniform int isActive;

void main() {
    if(hasTexture == 1){
        vec4 outFrag = texture(texture_sampler, outTexCoord);
        if (isActive == 0) {
            fragColor = texture(texture_sampler, outTexCoord);
        } else {
            fragColor = vec4(outFrag.r * 0.2, outFrag.g * 0.2, outFrag.b * 0.2, outFrag.w);
        }
    } else {
        fragColor = vec4(1.0, 0.5, 1.0, 1.0);
    }
}