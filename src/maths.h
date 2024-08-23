#ifndef _MATHS_H_
#define _MATHS_H_

// MATRICES ARE ROW-MAJOR, SO THE FLOATS GO LIKE A_00 A_01 A_02 A_03 A_10 A_11...
// OPENGL WANTS THEM TO BE COLUMN MAJOR, SO NEED TO TRANSPOSE

typedef struct
{
	float x, y;
} vec2, fvec2;

typedef struct
{
	int x, y;
} ivec2;

typedef struct
{
	union
	{
		struct
		{
			float x, y, z;
		};
		struct
		{
			float r, g, b;
		};
	};
} vec3, fvec3;

typedef struct
{
	union
	{
		struct
		{
			float x, y, z, w;
		};
		struct
		{
			float r, g, b, a;
		};
	};
} vec4, fvec4;

typedef struct
{
	union
	{
		float a[4];
		struct
		{
			float a00; float a01;
			float a10; float a11;
		};
	};
} mat2;

typedef struct
{
	union
	{
		float a[16];
		struct
		{
			float a00; float a01; float a02; float a03;
			float a10; float a11; float a12; float a13;
			float a20; float a21; float a22; float a23;
			float a30; float a31; float a32; float a33;
		};
	};
} mat4, fmat4;

typedef enum
{
	MATHS_ORIGIN_CENTRE,
	MATHS_ORIGIN_BOTTOM_LEFT,
} Maths_Origin;

typedef struct
{
	vec2 			position;
	vec2 			size;
	float 			angle;
	Maths_Origin 	origin;
} Quad;

typedef struct
{
	vec2			bottom_left;
	vec2			bottom_right;
	vec2			top_left;
	vec2			top_right;
} Quad_Vertices;

float			maths_deg_to_rad(float deg);

Quad_Vertices	maths_get_quad_vertcies(Quad q);

vec2			vec2_new(float x, float y);
vec3			vec3_new(float x, float y, float z);
vec4			vec4_new(float x, float y, float z, float w);

vec2			vec2_add_vec2(vec2 v1, vec2 v2);
vec2			vec2_sub_vec2(vec2 v1, vec2 v2);
vec2			vec2_mul_float(vec2 vec, float f);
vec2			vec2_reciprocal(vec2 vec);

int				ivec2_eq(ivec2 v1, ivec2 v2);

mat2			mat2_rotation_ccw(float angle);
mat2			mat2_rotation_cw(float angle);
vec2			mat2_mul_vec2(mat2 m, vec2 v);

mat4			mat4_identity();
mat4			mat4_custom_projection(vec2 virtual_size);

#endif