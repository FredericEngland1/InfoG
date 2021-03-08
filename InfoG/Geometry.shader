#version 460 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in FragmentData
{
	vec3 fNormal;
	vec3 fragmentPosition;
	vec2 textureCoordinates;
} fragmentDataIn[];

out FragmentData
{
	vec3 fNormal;
	vec3 fragmentPosition;
	vec2 textureCoordinates;
} fragmentDataOut;

void main()
{

	for (int i = 0; i < gl_in.length(); i++)
	{
		fragmentDataOut.fNormal = fragmentDataIn[i].fNormal;
		fragmentDataOut.fragmentPosition = fragmentDataIn[i].fragmentPosition;
		fragmentDataOut.fragmentPosition = fragmentDataIn[i].fragmentPosition;
		gl_Position = gl_in[i].gl_Position;
		EmitVertex();
		EndPrimitive();
	}
}