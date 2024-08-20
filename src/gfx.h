#ifndef _GFX_H_
#define _GFX_H_

#include "maths.h"

typedef struct
{
	int max_texture_units;
} Render_Info;

typedef struct
{
	Render_Info info;
} Gfx;


int gfx_init();
void gfx_shutdown();

void gfx_start_frame(vec4 bg_colour);
void gfx_end_frame();

#endif