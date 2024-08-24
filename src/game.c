#include "game.h"
#include "gfx.h"

#include <stdio.h>

Game game;
Registry reg;

int game_init()
{
	registry_init();

	for (int i = 0; i < 16; i++)
	{
		game.map.tiles[i].type = TILE_DIRT;
	}

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
	gfx_set_projection(GFX_BUILTIN_SHADER_TEXTURE, mat4_world_projection());
	
	// Render world

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			Tile *t;
			vec2 from_player;
			t = &game.map.tiles[y*4 + x];

			from_player = vec2_sub_vec2(v2((float)x, (float)y), game.position);

			gfx_draw_quad(gfx_quad_tex_bl(from_player, v2(1.0f, 1.0f), reg.tiles[t->type].sprite));
		}
	}

	gfx_flush_sprite_quads();
	gfx_set_projection(GFX_BUILTIN_SHADER_TEXTURE, mat4_ui_projection(v2(1280.0f, 720.0f)));

	// Render UI
}