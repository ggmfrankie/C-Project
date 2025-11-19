#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 texCoord;

out vec2 outTexCoord;
out vec3 mvVertexNormal;
out vec3 mvVertexPos;

uniform float width;
uniform float height;

uniform float screenWidth;
uniform float screenHeight;

uniform vec2 positionObject;

uniform int transformTexCoords;

uniform vec2 texPosStart;
uniform vec2 texPosEnd;

void main() {
    vec2 pixelPos = vec2(position.x * width, position.y * height);
    pixelPos += positionObject;

    vec2 normPos = vec2((pixelPos.x / screenWidth) * 2.0 - 1.0, 1.0 - (pixelPos.y / screenHeight) * 2.0);
    gl_Position = vec4(normPos, 0.0, 1.0);

    if(transformTexCoords == 1){
        outTexCoord = mix(texPosStart, texPosEnd, texCoord);
    } else {
        outTexCoord = texCoord;
    }
}