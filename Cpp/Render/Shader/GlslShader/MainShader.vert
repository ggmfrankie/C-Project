#version 460
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 uv;

out vec2 texCoords;

uniform mat4 transform;

void main()
{
    texCoords = uv;
    gl_Position = transform * vec4(pos, 1.0f);
}
