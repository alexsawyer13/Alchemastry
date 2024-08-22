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
	Quad quad;
	gfx_draw_quad(gfx_quad_colour_centred((vec2){200.0f, 200.0f}, (vec2){100.0f, 100.0f}, (vec4){0.0f, 1.0f, 0.0f, 1.0f}, 45.0f));

	quad.position = vec2_new(300.0f, 400.0f);
	quad.size = vec2_new(500.f, 100.0f);
	quad.colour = vec4_new(0.0f, 0.0f, 1.0f, 1.0f);
	quad.angle = maths_deg_to_rad(10.0f);
	quad.origin = GFX_ORIGIN_CENTRE;
	quad.type = GFX_QUAD_COLOUR;
	gfx_draw_quad(quad);
}