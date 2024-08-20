#version 330 core

layout (location = 0) in vec2 a_Pos;
layout (location = 1) in vec3 a_Colour;

out vec3 v_Colour;

void main()
{
	gl_Position = vec4(a_Pos.x*0.0015625 - 1.0, a_Pos.y*0.0027778 - 1.0, 0.0, 1.0);
	v_Colour = a_Colour;
}