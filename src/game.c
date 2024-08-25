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

	game.current_ui = GAME_UI_NONE;

	// Fill inventory

	for (int i = 0; i < 36; i++)
	{
		game.inventory[i].type = ITEM_NONE;
		game.inventory[i].count = 0;
	}

	game.inventory_hand.type = ITEM_NONE;
	game.inventory_hand.count = 0;

	game.current_hotbar_slot = 0;

	game.inventory[0].type = ITEM_WOOD;
	game.inventory[0].count = 10;

	game.inventory[1].type = ITEM_STONE;
	game.inventory[1].count = 15;

	game.inventory[8].type = ITEM_ELISHA;
	game.inventory[8].count = 1;

	// Generate map

	for (int i = 0; i < MAP_SIZE_A; i++)
	{
		int x;
		int y;

		y = i / MAP_SIZE_X;
		x = i % MAP_SIZE_X;

		game.map.tiles[i].type = TILE_NONE;
		game.map.tiles[i].foreground = FOREGROUND_NONE;

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

		if (game.map.tiles[i].type == TILE_STONE && platform_random() > 0.9f)
			game.map.tiles[i].foreground = FOREGROUND_ROCK;
		
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
	ivec2	mouse_inventory_slot;

	// Get mouse position in world and UI // TODO: IMRPOVE THIS

	mouse_position = platform_mouse_position();
	screen_size = platform_get_viewport_size();

	game.mouse_position_world.x = (((float)mouse_position.x / screen_size.x) * 2.0f - 1.0f) * settings.zoom;
	game.mouse_position_world.y = (((float)(screen_size.y - mouse_position.y) / screen_size.y) * 2 - 1) * settings.zoom * 9.0f / 16.0f;

	game.mouse_position_world = vec2_add_vec2(game.mouse_position_world, game.position);
	mouse_position_rounded = vec2_round_ivec2(game.mouse_position_world);

	game.mouse_tile_index = mouse_position_rounded.y * MAP_SIZE_X + mouse_position_rounded.x;

	game.mouse_position_ui.x = (float)mouse_position.x / screen_size.x * 1280.0f;
	game.mouse_position_ui.y = (float)(screen_size.y - mouse_position.y) / screen_size.y * 720.0f;

	mouse_inventory_slot = vec2_round_ivec2(vec2_mul_float(vec2_sub_vec2(game.mouse_position_ui, v2(UI_INVENTORY_POSITION_X, UI_INVENTORY_POSITION_Y)), 1.0f/UI_INVENTORY_CELL_SIZE));
	if (mouse_inventory_slot.x >= 0 && mouse_inventory_slot.x < 9 && mouse_inventory_slot.y >= 0 && mouse_inventory_slot.y < 4)
		game.mouse_inventory_index = mouse_inventory_slot.y * 9 + mouse_inventory_slot.x;		
	else
		game.mouse_inventory_index = -1;

	// game.mouse_inventory_index = mouse_inventory_slot.y * 9 + mouse_inventory_slot.x;
	// if (game.mouse_inventory_index < 0 || game.mouse_inventory_index >= 36)
		// game.mouse_inventory_index = -1;

	// Close window

	if (platform_key_down(GLFW_KEY_ESCAPE))
	{
		platform_window_close();
	}

	// Ui

	if (platform_key_pressed(GLFW_KEY_E))
 	{
		if (game.current_ui == GAME_UI_NONE)
		{
			game.current_ui = GAME_UI_INVENTORY;
		}

		else if (game.current_ui == GAME_UI_INVENTORY)
		{
			game.current_ui = GAME_UI_NONE;
		}
	}

	// Inventory

	if (game.current_ui == GAME_UI_INVENTORY)
	{
		if (platform_mouse_pressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			if (game.mouse_inventory_index != -1)
			{
				ItemStack tmp;
				tmp = game.inventory[game.mouse_inventory_index];
				game.inventory[game.mouse_inventory_index] = game.inventory_hand;
				game.inventory_hand = tmp;
			}
		}

		return;
	}

	// Hotbar

	for (int i = 1; i <= 9; i++)
	{
		if (platform_key_down(GLFW_KEY_0 + i))
		{
			game.current_hotbar_slot = i - 1;
		}
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

	if (platform_mouse_pressed(GLFW_MOUSE_BUTTON_LEFT))
	{
		game.map.tiles[game.mouse_tile_index].foreground = FOREGROUND_NONE;
	}

	if (platform_mouse_down(GLFW_MOUSE_BUTTON_RIGHT))
	{
		game.map.tiles[game.mouse_tile_index].type = TILE_PATH;
	}
}

void game_render()
{
	gfx_set_projection(GFX_BUILTIN_SHADER_TEXTURE, mat4_world_to_ndc_projection(settings.zoom));
	gfx_set_projection(GFX_BUILTIN_SHADER_COLOUR, mat4_world_to_ndc_projection(settings.zoom));
	
	// Render world

	//  Tiles
	for (int y = MAP_SIZE_X-1; y >= 0; y--)
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

	for (int y = MAP_SIZE_X-1; y >= 0; y--)
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

	// Hotbar
	
	for (int i = 0; i < 9; i++)
	{
		ItemStack stack;

		if (game.current_hotbar_slot == i)
			gfx_draw_quad(gfx_quad_tex_bl(v2(UI_HOTBAR_POSITION_X + i * UI_INVENTORY_CELL_SIZE, 0.0f), v2(UI_INVENTORY_CELL_SIZE, UI_INVENTORY_CELL_SIZE), reg.ui_hotbar_active_sprite));
		else
			gfx_draw_quad(gfx_quad_tex_bl(v2(UI_HOTBAR_POSITION_X + i * UI_INVENTORY_CELL_SIZE, 0.0f), v2(UI_INVENTORY_CELL_SIZE, UI_INVENTORY_CELL_SIZE), reg.ui_hotbar_sprite));
		
		stack = game.inventory[i];

		if (stack.count > 0)
		{
			gfx_draw_quad(gfx_quad_tex_bl(v2(370.0f + i * 60.0f + 10.0f, 10.0f), v2(40.0f, 40.0f), reg.items[stack.type].sprite));
		}
	}

	// Inventory

	if (game.current_ui == GAME_UI_INVENTORY)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int i = 0; i < 9; i++)
			{
				ItemStack stack;

				if (game.mouse_inventory_index == j*9 + i)
					gfx_draw_quad(gfx_quad_tex_bl(v2(UI_INVENTORY_POSITION_X + i * UI_INVENTORY_CELL_SIZE, UI_INVENTORY_POSITION_Y + UI_INVENTORY_CELL_SIZE * j), v2(UI_INVENTORY_CELL_SIZE, UI_INVENTORY_CELL_SIZE), reg.ui_hotbar_active_sprite));
				else
					gfx_draw_quad(gfx_quad_tex_bl(v2(UI_INVENTORY_POSITION_X + i * UI_INVENTORY_CELL_SIZE, UI_INVENTORY_POSITION_Y + UI_INVENTORY_CELL_SIZE * j), v2(UI_INVENTORY_CELL_SIZE, UI_INVENTORY_CELL_SIZE), reg.ui_hotbar_sprite));
				
				stack = game.inventory[j*9 + i];

				if (stack.count > 0)
				{
					gfx_draw_quad(gfx_quad_tex_bl(v2(UI_INVENTORY_POSITION_X + i * UI_INVENTORY_CELL_SIZE + UI_INVENTORY_ITEM_OFFSET, UI_INVENTORY_POSITION_Y + UI_INVENTORY_ITEM_OFFSET + UI_INVENTORY_CELL_SIZE * j), v2(UI_INVENTORY_ITEM_SIZE, UI_INVENTORY_ITEM_SIZE), reg.items[stack.type].sprite));
				}
			}
		}

		if (game.inventory_hand.count > 0 && game.inventory_hand.type != ITEM_NONE)
			gfx_draw_quad(gfx_quad_tex_centred(game.mouse_position_ui, v2(UI_INVENTORY_ITEM_SIZE, UI_INVENTORY_ITEM_SIZE), reg.items[game.inventory_hand.type].sprite));
	}
}