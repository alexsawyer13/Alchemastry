#include "game.h"
#include "gfx.h"

int game_init()
{
	return 1;
}

void game_shutdown()
{

}

void game_update()
{

}

void game_render()
{
	ColouredQuad quad;
	quad.position = vec2_new(200.0f, 200.0f);
	quad.size = vec2_new(100.0f, 100.0f);
	quad.colour = vec4_new(0.0f, 1.0f, 0.0f, 1.0f);
	quad.angle = maths_deg_to_rad(45.0f);
	quad.origin = GFX_ORIGIN_CENTRE;
	gfx_draw_quad(&quad);

	quad.position = vec2_new(300.0f, 400.0f);
	quad.size = vec2_new(500.f, 100.0f);
	quad.colour = vec4_new(0.0f, 0.0f, 1.0f, 1.0f);
	quad.angle = maths_deg_to_rad(10.0f);
	gfx_draw_quad(&quad);
}