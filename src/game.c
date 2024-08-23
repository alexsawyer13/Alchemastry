#include "game.h"
#include "gfx.h"

#include <stdio.h>

Game game;

Texture tex[3];

Sprite elisha;
Sprite map;
Sprite cat;

int game_init()
{
	gfx_texture_create(&tex[0], "assets/maps/elisha.jpg");
	gfx_texture_create(&tex[1], "assets/maps/test_map.png");
	gfx_texture_create(&tex[2], "assets/maps/cat.jpg");

	gfx_texture_bind(&tex[0], 0);
	gfx_texture_bind(&tex[1], 1);
	gfx_texture_bind(&tex[2], 2);

	printf("%d, %d, %d\n", tex[0].id, tex[1].id, tex[2].id);

	elisha.texture = tex[0];
	elisha.tex_coord_origin = vec2_new(0.0f, 0.0f);
	elisha.tex_coord_size = vec2_new(1.0f, 1.0f);

	map.texture = tex[1];
	map.tex_coord_origin = vec2_new(0.0f, 0.0f);
	map.tex_coord_size = vec2_new(1.0f, 1.0f);

	cat.texture = tex[2];
	cat.tex_coord_origin = vec2_new(0.0f, 0.0f);
	cat.tex_coord_size = vec2_new(1.0f, 1.0f);

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
	gfx_draw_quad(gfx_quad_colour_centred(
		(vec2){100.0f, 100.0f},
		(vec2){100.0f, 100.0f},
		(vec4){0.0f, 1.0f, 0.0f, 1.0f},
		0.0f
	));

	Gfx_Quad quad;
	quad.quad.position = vec2_new(640.0f, 360.0f);
	quad.quad.size = vec2_new(720.0f, 360.0f);
	quad.quad.angle = maths_deg_to_rad(0.0f);
	quad.quad.origin = MATHS_ORIGIN_BOTTOM_LEFT;
	quad.type = GFX_QUAD_SPRITE;
	quad.sprite = cat;
	gfx_draw_quad(quad);

	quad.quad.position = vec2_new(0.0f, 360.0f);
	quad.sprite = elisha;
	gfx_draw_quad(quad);

	quad.quad.position = vec2_new(720.0f, 0.0f);
	quad.sprite = map;
	gfx_draw_quad(quad);
}