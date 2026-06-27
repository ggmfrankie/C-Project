
#version 460

in vec2 outTexCoord;
in vec3 mvVertexNormal;
in vec3 mvVertexPos;
in float outPatchFlag;

//in bool canStretchHorizontal;
//in bool canStretchVertical;

out vec4 fragColor;



uniform sampler2D texture_sampler;
uniform int isActive;
uniform int hasTexture;



void main() {
    //if(outFrag.a == 0.0) discard;

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
    //fragColor = vec4(1.0, outPatchFlag/3, 0.0, 1.0);
}
