#include "platform.h"
#include "gfx.h"

#include <stdio.h>

int main()
{
	platform_init();
	gfx_init();

	while (!platform_window_closed())
	{
		gfx_start_frame((vec4) { 1.0f, 0.0f, 0.0f, 1.0f });
		gfx_end_frame();

		platform_update();
	}

	gfx_shutdown();
	platform_shutdown();

	return 0;
}