#include "maths.h"

#include <math.h>

vec2 vec2_new(float x, float y)
{
	return (vec2) { x, y };
}

vec3 vec3_new(float x, float y, float z)
{
	return (vec3) { x, y, z };
}

vec4 vec4_new(float x, float y, float z, float w)
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

mat4 mat4_custom_projection(vec2 virtual_size)
{
	mat4 mat;
	vec2 sf;
	sf = vec2_reciprocal(vec2_mul_float(virtual_size, 0.5f));
	mat.a[0] = sf.x;	mat.a[1] = 0.0f;	mat.a[2] = 0.0f;	mat.a[3] = -1.0f;
	mat.a[4] = 0.0f;	mat.a[5] = sf.y;	mat.a[6] = 0.0f;	mat.a[7] = -1.0f;
	mat.a[8] = 0.0f;	mat.a[9] = 0.0f;	mat.a[10] = 1.0f;	mat.a[11] = 0.0f;
	mat.a[12] = 0.0f;	mat.a[13] = 0.0f;	mat.a[14] = 0.0f;	mat.a[15] = 1.0f;
	return mat;
}

int ivec2_eq(ivec2 v1, ivec2 v2)
{
	return ((v1.x == v2.x) && (v1.y == v2.y));
}

vec2 vec2_add_vec2(vec2 v1, vec2 v2)
{
	return (vec2) { v1.x + v2.x, v1.y + v2.y };
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