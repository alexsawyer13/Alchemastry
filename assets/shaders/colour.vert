#version 330 core

layout (location = 0) in vec2 a_Pos;
layout (location = 1) in vec3 a_Colour;

out vec3 v_Colour;

uniform mat4 u_Projection;

void main()
{
	gl_Position = u_Projection * vec4(a_Pos, 0.0, 1.0);
	v_Colour = a_Colour;
}