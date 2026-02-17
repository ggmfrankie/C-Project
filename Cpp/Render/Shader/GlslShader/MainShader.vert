#version 460
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normals;
layout (location = 2) in vec2 texCords;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
