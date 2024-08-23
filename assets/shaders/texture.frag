#version 330 core

in vec2 v_TexCoord;

out vec4 o_Colour;

uniform sampler2D u_Texture[ 3];

void main()
{
	o_Colour = texture(u_Texture[0], v_TexCoord);
}