#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 vertexNormal;
layout (location = 3) in int patchFlag;

out vec2 outTexCoord;
out vec3 mvVertexNormal;
out vec3 mvVertexPos;
out float outPatchFlag;

struct Stretch{
    float stretchStartX;
    float stretchStartY;
    float stretchEndX;
    float stretchEndY;
};

struct Scale{
    float left;
    float right;
    float top;
    float bottom;
};

uniform float width;
uniform float height;

uniform float screenWidth;
uniform float screenHeight;

uniform float textureHeigth;
uniform float textureWidth;

uniform vec2 positionObject;

uniform Stretch patchStretch;
uniform Scale scale;

vec2 scaledPosition(vec2 pos);

void main() {

    #define DEBUG_UNIFORMS

    #ifdef DEBUG_UNIFORMS
    float dummy = patchStretch.stretchEndY + patchStretch.stretchEndX + patchStretch.stretchStartX + patchStretch.stretchStartY;
    dummy += scale.bottom + scale.left + scale.right + scale.top;
    #endif

    vec2 normPos = position.xy;
    vec2 pixelPos = scaledPosition(normPos);
    pixelPos += positionObject;

    if(patchFlag == -909){

    }   else if(patchFlag == 1){
        //pixelPos.x += 20.0;
        pixelPos.x += (scale.left);
    }   else if(patchFlag == -1){
        //pixelPos.x += -20.0;
        pixelPos.x += (scale.right);

    }   else if(patchFlag == 2){
        //pixelPos.y += 20.0;
        pixelPos.y += (scale.top);
    }   else if(patchFlag == -2){
        //pixelPos.y += -20.0;
        pixelPos.y += (scale.bottom);

    }   else if(patchFlag == 3){
        //pixelPos.x += 20.0;
        //pixelPos.y += 20.0;
        pixelPos.x += (scale.left);
        pixelPos.y += (scale.top);
    }   else if(patchFlag == 4){
        //pixelPos.x += -20.0;
        //pixelPos.y += 20.0;
        pixelPos.x += (scale.right);
        pixelPos.y += (scale.top);
    }   else if(patchFlag == 5){
        //pixelPos.x += 20.0;
        //pixelPos.y += -20.0;
        pixelPos.x += (scale.left);
        pixelPos.y += (scale.bottom);
    }   else if(patchFlag == 6){
        //pixelPos.x += -20.0;
        //pixelPos.y += -20.0;
        pixelPos.x += (scale.right);
        pixelPos.y += (scale.bottom);
    }

    vec2 ndcPos = vec2(
    (pixelPos.x / screenWidth) * 2.0 - 1.0,
    1.0 - (pixelPos.y / screenHeight) * 2.0);

    gl_Position = vec4(ndcPos, 0.0, 1.0);
    outTexCoord = texCoord;
    mvVertexNormal = vertexNormal;
    mvVertexPos = position;
    outPatchFlag = patchFlag;
}

vec2 scaledPosition(vec2 pos) {
    return vec2(pos.x * width,
    pos.y * height);
}





