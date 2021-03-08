#version 460 core

in vec4 vPosition;
in vec4 vNormal;
in vec2 vTextureCoordinates;

out FragmentData
{
	vec3 fNormal;
	vec3 fragmentPosition;
	vec2 textureCoordinates;
} fragmentDataOut;

uniform mat4 MV;
uniform mat4 MVP;
uniform mat4 normalMatrix;

void main()
{
	fragmentDataOut.fNormal = vec3(normalize(normalMatrix * vNormal));
	fragmentDataOut.fragmentPosition = vec3(MV * vPosition);
	fragmentDataOut.textureCoordinates = vTextureCoordinates;

	gl_Position = MVP * vPosition;
}