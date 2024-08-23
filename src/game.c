#include "game.h"
#include "gfx.h"

#include <stdio.h>

Game game;

Texture tex;
Sprite sprite;

int game_init()
{
	gfx_texture_create(&tex, "assets/maps/elisha.jpg");

	sprite.texture = &tex;
	sprite.tex_coord_origin = vec2_new(0.0f, 0.0f);
	sprite.tex_coord_size = vec2_new(1.0f, 1.0f);

	printf("DEBUG: TEXTURE ID %d\n", tex.id);

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
	quad.sprite = sprite;
	gfx_draw_quad(quad);
}