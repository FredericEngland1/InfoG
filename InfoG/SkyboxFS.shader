#version 460 core

in vec3 textureCoordinates;
out vec4 fragColor;

uniform samplerCube skyboxCubemap;

void main()
{
    fragColor = texture(skyboxCubemap, textureCoordinates);
}