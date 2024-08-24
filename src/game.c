#include "game.h"
#include "gfx.h"
#include "platform.h"

#include <stdio.h>

Game 		game;
Settings 	settings;
Registry 	reg;

int game_init()
{
	registry_init();

	settings.zoom = 12.0f;

	game.position = v2(128.0f, 128.0f);
	game.speed = 5.0f;

	for (int i = 0; i < MAP_SIZE_A; i++)
	{
		int x;
		int y;

		y = i / MAP_SIZE_X;
		x = i % MAP_SIZE_X;

		if (platform_random() > 0.5f)
			game.map.tiles[i].type = TILE_GRASS;
		else
			game.map.tiles[i].type = TILE_DIRT;

		if (x == 128.0f)
			game.map.tiles[i].type = TILE_STONE;

		if (y == 128.0f)
			game.map.tiles[i].type = TILE_PATH;

		if (game.map.tiles[i].type == TILE_GRASS && platform_random() > 0.95f)
			game.map.tiles[i].foreground = FOREGROUND_TREE;
		else
			game.map.tiles[i].foreground = FOREGROUND_NONE;
		
	}

	return 1;
}

void game_shutdown()
{

}

void game_update()
{
	vec2 	player_direction;
	ivec2 	player_tile_position;
	int		player_tile_index;

	vec2 	mouse_position;
	ivec2 	mouse_position_rounded;
	ivec2 	screen_size;

	// Get mouse position in world // TODO: IMRPOVE THIS

	mouse_position = platform_mouse_position();
	screen_size = platform_get_viewport_size();

	game.mouse_position_world.x = (((float)mouse_position.x / screen_size.x) * 2.0f - 1.0f) * settings.zoom;
	game.mouse_position_world.y = (((float)(screen_size.y - mouse_position.y) / screen_size.y) * 2 - 1) * settings.zoom * 9.0f / 16.0f;

	game.mouse_position_world = vec2_add_vec2(game.mouse_position_world, game.position);
	mouse_position_rounded = vec2_round_ivec2(game.mouse_position_world);

	game.mouse_tile_index = mouse_position_rounded.y * MAP_SIZE_X + mouse_position_rounded.x;

	// Close window

	if (platform_key_down(GLFW_KEY_ESCAPE))
	{
		platform_window_close();
	}

	// Player movement

	player_tile_position = vec2_round_ivec2(game.position);
	player_tile_index = player_tile_position.y * MAP_SIZE_X + player_tile_position.x;

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
		game.position = vec2_add_vec2(game.position, vec2_mul_float(vec2_normalise(player_direction), game.speed * reg.tiles[game.map.tiles[player_tile_index].type].speed_multiplier * platform_delta_time()));

	// Click
	if (platform_key_pressed(GLFW_KEY_E))
	{
		if (game.map.tiles[game.mouse_tile_index].foreground == FOREGROUND_TREE)
		{
			game.map.tiles[game.mouse_tile_index].foreground = FOREGROUND_NONE;
		}
	}
}

void game_render()
{
	gfx_set_projection(GFX_BUILTIN_SHADER_TEXTURE, mat4_world_to_ndc_projection(settings.zoom));
	gfx_set_projection(GFX_BUILTIN_SHADER_COLOUR, mat4_world_to_ndc_projection(settings.zoom));
	
	// Render world

	//  Tiles
	for (int y = 0; y < MAP_SIZE_X; y++)
	{
		for (int x = 0; x < MAP_SIZE_X; x++)
		{
			Tile *t;
			vec2 from_player;

			t = &game.map.tiles[y*MAP_SIZE_X + x];

			from_player = vec2_sub_vec2(v2((float)x, (float)y), game.position);

			if (y*MAP_SIZE_X + x == game.mouse_tile_index)
				gfx_draw_quad(gfx_quad_tex_bl(from_player, v2(1.0f, 1.0f), reg.tiles[TILE_NONE].sprite));
			else
				gfx_draw_quad(gfx_quad_tex_bl(from_player, v2(1.0f, 1.0f), reg.tiles[t->type].sprite));
		}
	}

	// Foreground elements
	for (int y = 0; y < MAP_SIZE_X; y++)
	{
		for (int x = 0; x < MAP_SIZE_X; x++)
		{
			Tile *t;
			ForegroundInfo *f;
			vec2 from_player;

			t = &game.map.tiles[y*MAP_SIZE_X + x];
			f = &reg.foregrounds[t->foreground];

			from_player = vec2_sub_vec2(v2((float)x, (float)y), game.position);

			if (t->foreground != FOREGROUND_NONE)
				gfx_draw_quad(gfx_quad_tex_bl(from_player, f->size, f->sprite));
		}
	}

	gfx_draw_quad(gfx_quad_tex_centred(v2(0.0f, 0.0f), v2(0.8, 1.6f), reg.player_sprite));

	gfx_flush_sprite_quads();
	gfx_set_projection(GFX_BUILTIN_SHADER_TEXTURE, mat4_ui_projection(v2(1280.0f, 720.0f)));
	gfx_set_projection(GFX_BUILTIN_SHADER_COLOUR, mat4_ui_projection(v2(1280.0f, 720.0f)));

	// Render UI
}