#include "gfx.h"
#include "platform.h"

#include <glad/glad.h>

Gfx gfx;

int gfx_init()
{
	gladLoadGLLoader(platform_get_gladloadproc());

	return 1;
}

void gfx_shutdown()
{

}

void gfx_start_frame(vec4 bg_colour)
{
	glClearColor(bg_colour.r, bg_colour.g, bg_colour.b, bg_colour.a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void gfx_end_frame()
{

}