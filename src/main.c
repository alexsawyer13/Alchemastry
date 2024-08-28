#include "platform.h"
#include "gfx.h"
#include "game.h"

int main()
{
	if (!platform_init()) return 1;
	if (!gfx_init()) return 1;
	if (!game_init()) return 1;

	while (!platform_window_closed())
	{
		game_update();

		gfx_start_frame((vec4) { 1.0f, 0.0f, 1.0f, 1.0f });
		game_render();
		gfx_end_frame();

		platform_update();
	}

	game_shutdown();
	gfx_shutdown();
	platform_shutdown();

	return 0;
}
