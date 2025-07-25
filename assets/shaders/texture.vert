#version 330 core
layout (location = 0) in vec2 a_Pos;
layout (location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;

uniform mat4 u_Projection;

void main()
{
    gl_Position = u_Projection * vec4(a_Pos, 0.0, 1.0);
    v_TexCoord = a_TexCoord;
}