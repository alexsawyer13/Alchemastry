#version 330 core

in vec3 v_Colour;

out vec4 o_Colour;

void main()
{
	o_Colour = vec4(v_Colour, 1.0);
}