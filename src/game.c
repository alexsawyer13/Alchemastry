#include "game.h"
#include "gfx.h"

#include <stdio.h>

Game game;

Texture tex[2];
Sprite sprite[2];

int game_init()
{
	gfx_texture_create(&tex[0], "assets/maps/elisha.jpg");
	gfx_texture_create(&tex[1], "assets/maps/test_map.png");

	gfx_texture_bind(&tex[0], 0);
	gfx_texture_bind(&tex[1], 1);

	sprite[0].texture = &tex[0];
	sprite[0].tex_coord_origin = vec2_new(0.0f, 0.0f);
	sprite[0].tex_coord_size = vec2_new(1.0f, 1.0f);

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
	gfx_draw_quad(gfx_quad_colour_centred((vec2){100.0f, 100.0f}, (vec2){100.0f, 100.0f}, (vec4){0.0f, 1.0f, 0.0f, 1.0f}, 0.0f));

	Gfx_Quad quad;
	// quad.position = vec2_new(-0.9f, -0.9f);
	// quad.size = vec2_new(1.8f, 1.8f);
	quad.quad.position = vec2_new(640.0f, 360.0f);
	quad.quad.size = vec2_new(640.0f, 360.0f);
	quad.quad.angle = 45.0f;
	quad.quad.origin = GFX_ORIGIN_CENTRE;
	quad.type = GFX_QUAD_SPRITE;
	quad.sprite = sprite[0];
	gfx_draw_quad(quad);
}