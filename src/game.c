#include "game.h"
#include "gfx.h"
#include "platform.h"

#include <stdio.h>

Game game;
Registry reg;

int game_init()
{
	registry_init();

	game.position = v2(0.0f, 0.0f);
	game.speed = 5.0f;

	for (int i = 0; i < MAP_SIZE_A; i++)
	{
		if (platform_random() > 0.5f)
			game.map.tiles[i].type = TILE_GRASS;
		else
			game.map.tiles[i].type = TILE_DIRT;
	}

	return 1;
}

void game_shutdown()
{

}

void game_update()
{
	vec2 player_direction;

	player_direction = v2(0.0f, 0.0f);
	
	if (platform_key_down(GLFW_KEY_W))
		player_direction.y += 1.0f;
	if (platform_key_down(GLFW_KEY_S))
		player_direction.y -= 1.0f;
	if (platform_key_down(GLFW_KEY_D))
		player_direction.x += 1.0f;
	if (platform_key_down(GLFW_KEY_A))
		player_direction.x -= 1.0f;
	
	if (player_direction.x != 0.0f && player_direction.y != 0.0f);
		game.position = vec2_add_vec2(game.position, vec2_mul_float(vec2_normalise(player_direction), game.speed * platform_delta_time()));

	if (platform_key_down(GLFW_KEY_ESCAPE))
	{
		platform_window_close();
	}
}

void game_render()
{
	gfx_set_projection(GFX_BUILTIN_SHADER_TEXTURE, mat4_world_projection());
	gfx_set_projection(GFX_BUILTIN_SHADER_COLOUR, mat4_world_projection());
	
	// Render world

	for (int y = 0; y < MAP_SIZE_X; y++)
	{
		for (int x = 0; x < MAP_SIZE_X; x++)
		{
			Tile *t;
			vec2 from_player;
			t = &game.map.tiles[y*MAP_SIZE_X + x];

			from_player = vec2_sub_vec2(v2((float)x, (float)y), game.position);

			//gfx_draw_quad(gfx_quad_colour_centred(from_player, v2(1.0f, 1.0f), v4(1.0f, 0.0f, 0.0f, 1.0f), 0.0f));
			gfx_draw_quad(gfx_quad_tex_bl(from_player, v2(1.0f, 1.0f), reg.tiles[t->type].sprite));
		}
	}

	gfx_flush_sprite_quads();
	gfx_set_projection(GFX_BUILTIN_SHADER_TEXTURE, mat4_ui_projection(v2(1280.0f, 720.0f)));

	// Render UI
}