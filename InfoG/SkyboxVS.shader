#version 460 core

layout(location = 0) in vec3 position;
out vec3 textureCoordinates;

uniform mat4 VP;

void main()
{
    textureCoordinates = position;
    gl_Position = VP * vec4(position, 1.0);
}