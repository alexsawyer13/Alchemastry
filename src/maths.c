#include "maths.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

vec2 v2(float x, float y)
{
	return (vec2) { x, y };
}

vec3 vec3_new(float x, float y, float z)
{
	return (vec3) { x, y, z };
}

vec4 v4(float x, float y, float z, float w)
{
	return (vec4) { x, y, z, w };
}

vec2 vec2_mul_float(vec2 vec, float f)
{
	return (vec2) { vec.x * f, vec.y * f };
}

vec2 vec2_reciprocal(vec2 vec)
{
	return (vec2) { 1.0f / vec.x, 1.0f / vec.y };
}

mat4 mat4_identity()
{
	mat4 mat;
	mat.a[0]  = 1.0f; mat.a[1]  = 0.0f; mat.a[2]  = 0.0f; mat.a[3] =  0.0f;
	mat.a[4]  = 0.0f; mat.a[5]  = 1.0f; mat.a[6]  = 0.0f; mat.a[7] =  0.0f;
	mat.a[8]  = 0.0f; mat.a[9]  = 0.0f; mat.a[10] = 1.0f; mat.a[11] = 0.0f;
	mat.a[12] = 0.0f; mat.a[13] = 0.0f; mat.a[14] = 0.0f; mat.a[15] = 1.0f;
	return mat;
}

mat4 mat4_world_to_ndc_projection(float screen_width_tiles)
{
	mat4 m;
	vec2 sf;
	float screen_height_tiles;

	screen_height_tiles = 9.0f / 16.0f * screen_width_tiles;

	sf.x = 1.0f / screen_width_tiles;
	sf.y = 1.0f / screen_height_tiles;

	m.a[0] = sf.x;	m.a[1] = 0.0f;	m.a[2] = 0.0f;	m.a[3] = 0.0f;
	m.a[4] = 0.0f;	m.a[5] = sf.y;	m.a[6] = 0.0f;	m.a[7] = 0.0f;
	m.a[8] = 0.0f;	m.a[9] = 0.0f;	m.a[10] = 1.0f;	m.a[11] = 0.0f;
	m.a[12] = 0.0f;	m.a[13] = 0.0f;	m.a[14] = 0.0f;	m.a[15] = 1.0f;

	return m;
}

mat4 mat4_ui_projection(vec2 virtual_size)
{
	mat4 m;
	vec2 sf;
	sf = vec2_reciprocal(vec2_mul_float(virtual_size, 0.5f));
	m.a[0] = sf.x;	m.a[1] = 0.0f;	m.a[2] = 0.0f;	m.a[3] = -1.0f;
	m.a[4] = 0.0f;	m.a[5] = sf.y;	m.a[6] = 0.0f;	m.a[7] = -1.0f;
	m.a[8] = 0.0f;	m.a[9] = 0.0f;	m.a[10] = 1.0f;	m.a[11] = 0.0f;
	m.a[12] = 0.0f;	m.a[13] = 0.0f;	m.a[14] = 0.0f;	m.a[15] = 1.0f;
	return m;
}

int ivec2_eq(ivec2 v1, ivec2 v2)
{
	return ((v1.x == v2.x) && (v1.y == v2.y));
}

vec2 vec2_add_vec2(vec2 v1, vec2 v2)
{
	return (vec2) { v1.x + v2.x, v1.y + v2.y };
}

vec2 vec2_sub_vec2(vec2 v1, vec2 v2)
{
	return (vec2) { v1.x - v2.x, v1.y - v2.y };
}

mat2 mat2_rotation_ccw(float angle)
{
	mat2 mat;
	mat.a00 = cos(angle);	mat.a01 = -sin(angle);
	mat.a10 = sin(angle);	mat.a11 = cos(angle);
	return mat;
}

vec2 mat2_mul_vec2(mat2 m, vec2 v)
{
	return (vec2) { m.a00 * v.x + m.a01 * v.y, m.a10 * v.x + m.a11 * v.y };
}

float maths_deg_to_rad(float deg)
{
	return deg * 0.017453f;
}

mat2 mat2_rotation_cw(float angle)
{
	mat2 mat;
	mat.a00 = cos(angle);	mat.a01 = sin(angle);
	mat.a10 = -sin(angle);	mat.a11 = cos(angle);
	return mat;
}

Quad_Vertices maths_get_quad_vertcies(Quad q)
{
	Quad_Vertices v;
	mat2 rotation;
	
	rotation = mat2_rotation_ccw(q.angle);

	switch(q.origin)
	{
		case MATHS_ORIGIN_CENTRE:
		{
			vec2 half_size;
			vec2 top_offset;
			vec2 right_offset;

			half_size = vec2_mul_float(q.size, 0.5f);

			top_offset = mat2_mul_vec2(rotation, v2(0.0f, half_size.y));
			right_offset = mat2_mul_vec2(rotation, v2(half_size.x, 0.0f));

			v.bottom_left = vec2_sub_vec2(q.position, vec2_add_vec2(top_offset, right_offset));
			v.bottom_right = vec2_add_vec2(q.position, vec2_sub_vec2(right_offset, top_offset));
			v.top_left = vec2_add_vec2(q.position, vec2_sub_vec2(top_offset, right_offset));
			v.top_right = vec2_add_vec2(q.position, vec2_add_vec2(top_offset, right_offset));
		}
		break;
		case MATHS_ORIGIN_BOTTOM_LEFT:
		{
			vec2 top_offset;
			vec2 right_offset;

			top_offset = mat2_mul_vec2(rotation, v2(0.0f, q.size.y));
			right_offset = mat2_mul_vec2(rotation, v2(q.size.x, 0.0f));

			v.bottom_left = q.position;
			v.bottom_right = vec2_add_vec2(q.position, right_offset);
			v.top_left = vec2_add_vec2(q.position, top_offset);
			v.top_right = vec2_add_vec2(q.position, vec2_add_vec2(top_offset, right_offset));
		}
		break;
		default:
			fprintf(stderr, "Trying to get vertices of quad type %d which doesn't exist\n", q.origin);
			v.bottom_left = v2(0.0f, 0.0f);
			v.bottom_right = v2(0.0f, 0.0f);
			v.top_left = v2(0.0f, 0.0f);
			v.top_right = v2(0.0f, 0.0f);
		break;
	}

	return v;
}

vec2 vec2_normalise(vec2 vec)
{
	float inv_norm;
	if (vec.x == 0.0f && vec.y == 0.0f) return vec;
	inv_norm = 1.0f / sqrt(vec.x * vec.x + vec.y * vec.y);
	return vec2_mul_float(vec, inv_norm);
}

ivec2 iv2(int x, int y)
{
	return (ivec2) {x, y};
}

ivec2 vec2_round_ivec2(vec2 vec)
{
	return (ivec2){(int)vec.x, (int)vec.y};
}