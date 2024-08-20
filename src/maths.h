#ifndef _MATHS_H_
#define _MATHS_H_

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

#endif