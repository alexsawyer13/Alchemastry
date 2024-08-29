#include "game.h"
#include "gfx.h"
#include "platform.h"
#include "log.h"

#include <math.h>

Game 		game;
Settings 	settings;
Registry 	reg;

int game_init()
{
	registry_init();

	settings.zoom = 12.0f;

	game.position = v2(128.0f, 128.0f);
	game.speed = 5.0f;
	game.reach = 3.5f;

	game.current_ui = GAME_UI_NONE;

	// Fill inventory

	for (int i = 0; i < 36; i++)
	{
		game.inventory[i].type = ITEM_NONE;
		game.inventory[i].count = 0;
	}

	for (int i = 0; i < GAME_MAX_GROUND_ITEM_COUNT; i++)
	{
		game.ground_items[i].stack.type = ITEM_NONE;
		game.ground_items[i].stack.count = 0;
		game.ground_items[i].position = v2(0.0f, 0.0f);
	}

	game.ground_item_count = 0;
	game.ground_item_index = 0;

	game.inventory_hand.type = ITEM_NONE;
	game.inventory_hand.count = 0;

	game.current_hotbar_slot = 0;

	game.inventory[0].type = ITEM_WOOD;
	game.inventory[0].count = 999;

	game.inventory[1].type = ITEM_STONE;
	game.inventory[1].count = 15;
	
	game.inventory[2].type = ITEM_PATH;
	game.inventory[2].count = 999;

	game.inventory[35].type = ITEM_ELISHA;
	game.inventory[35].count = 1;

	// Generate map

	for (int i = 0; i < MAP_SIZE_A; i++)
	{
		Map_Tile *tile;
		int x;
		int y;

		tile = &game.map.tiles[i];
		y = i / MAP_SIZE_X;
		x = i % MAP_SIZE_X;

		tile->background = TILE_NONE;
		tile->foreground = TILE_NONE;
		tile->ground_object = GROUND_OBJECT_NONE;

		if (platform_random() > 0.5f)
			tile->background = TILE_GRASS;
		else
			tile->background = TILE_DIRT;

		if (x == 128.0f)
			tile->background = TILE_STONE;

		if (y == 128.0f)
			tile->foreground = TILE_PATH;

		if (game_get_top_tile_from_maptile(tile) == TILE_GRASS && platform_random() > 0.95f)
			tile->ground_object = GROUND_OBJECT_TREE;

		if (game_get_top_tile_from_maptile(tile) == TILE_STONE && platform_random() > 0.9f)
			tile->ground_object = GROUND_OBJECT_ROCK;
		
	}

	return 1;
}

void game_shutdown()
{
	registry_shutdown();
}

void game_update()
{
	vec2 	player_direction;
	ivec2 	player_tile_position;
	int	player_tile_index;

	vec2 	mouse_position;
	ivec2 	screen_size;
	ivec2	mouse_inventory_slot;

	int	ground_items_updated;

	// Get mouse position in world and UI

	mouse_position = platform_mouse_position();
	screen_size = platform_get_viewport_size();

	game.mouse_position_world.x = (((float)mouse_position.x / screen_size.x) * 2.0f - 1.0f) * settings.zoom;
	game.mouse_position_world.y = (((float)(screen_size.y - mouse_position.y) / screen_size.y) * 2 - 1) * settings.zoom * 9.0f / 16.0f;

	game.mouse_position_world = vec2_add_vec2(game.mouse_position_world, game.position);
	game.mouse_tile = vec2_round_ivec2(game.mouse_position_world);

	game.mouse_tile_index = game.mouse_tile.y * MAP_SIZE_X + game.mouse_tile.x;

	game.mouse_position_ui.x = (float)mouse_position.x / screen_size.x * 1280.0f;
	game.mouse_position_ui.y = (float)(screen_size.y - mouse_position.y) / screen_size.y * 720.0f;

	mouse_inventory_slot = vec2_round_ivec2(vec2_mul_float(vec2_sub_vec2(game.mouse_position_ui, v2(UI_INVENTORY_POSITION_X, UI_INVENTORY_POSITION_Y)), 1.0f/UI_INVENTORY_CELL_SIZE));
	if (mouse_inventory_slot.x >= 0 && mouse_inventory_slot.x < 9 && mouse_inventory_slot.y >= 0 && mouse_inventory_slot.y < 4)
		game.mouse_inventory_index = mouse_inventory_slot.y * 9 + mouse_inventory_slot.x;		
	else
		game.mouse_inventory_index = -1;

	// Close window

	if (platform_key_down(GLFW_KEY_ESCAPE))
	{
		platform_window_close();
	}

	// UI

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
				Item_Stack tmp;
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


	// Click

	if (platform_mouse_pressed(GLFW_MOUSE_BUTTON_LEFT))
	{
		Map_Tile *mouse_tile;
		Ground_Object_Type ground_object_type;
		Ground_Object_Info *ground_object_info;

		mouse_tile = game_get_maptile(game.mouse_tile);
		ground_object_type = mouse_tile->ground_object;
		ground_object_info = GROUND_OBJECT_INFO(ground_object_type);
		mouse_tile->ground_object = GROUND_OBJECT_NONE;

		if (ground_object_type != GROUND_OBJECT_NONE)
		{
			Drop_Stack drop;

			drop = game_droptable(ground_object_info->droptable);

			for (int i = 0; i < drop.stack_count; i++)
			{
				Ground_Item item;

				item.position = v2((float)(game.mouse_tile.x), (float)(game.mouse_tile.y));
				item.stack = drop.stacks[i];
				item.player_detection_time = 0.0f;

				game_add_ground_item(&item);
			}
		}

		else if (mouse_tile->foreground != TILE_NONE)
		{
			mouse_tile->foreground = TILE_NONE;
		}
	}

	if (platform_mouse_down(GLFW_MOUSE_BUTTON_RIGHT))
	{
		Item_Stack *hand;

		hand = game_get_handitem();

		if (hand->count > 0)
		{
			if (game_place_item(game.mouse_tile, hand->type))
			{
				hand->count--;
			}
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
	
	if (player_direction.x != 0.0f || player_direction.y != 0.0f)
		game.position = vec2_add_vec2(game.position, vec2_mul_float(vec2_normalise(player_direction), game.speed * TILE_INFO(game_get_top_tile(player_tile_position))->speed_multiplier * platform_delta_time()));

	// Update ground items
	
	ground_items_updated = 0;
	for (int i = 0; i < GAME_MAX_GROUND_ITEM_COUNT; i++)
	{
		Ground_Item	*item;
		vec2		to_player;
		float		dist_to_player_sqr;

		item = &game.ground_items[i];
		if (item->stack.count == 0 || item->stack.type == ITEM_NONE)
			continue;

		to_player = vec2_sub_vec2(game.position, item->position);
		dist_to_player_sqr = vec2_length_sqr(to_player);

		if (item->player_detection_time == 0.0f && dist_to_player_sqr <= game.reach * game.reach)
		{
			item->player_detection_time = platform_time();
		}

		if (item->player_detection_time != 0.0f)
		{
			float time_to_reach_player;
			float speed_to_reach_player;

			time_to_reach_player = item->player_detection_time + GAME_GROUND_ITEM_SPEED - platform_time();

			if (time_to_reach_player <= 0.05)
			{
				item->position = game.position;
			}
			else
			{
				speed_to_reach_player = sqrtf(dist_to_player_sqr) / time_to_reach_player;

				item->position = vec2_add_vec2(item->position, vec2_mul_float(vec2_normalise(to_player), speed_to_reach_player * platform_delta_time()));
			}
		}

		if (vec2_eq_vec2(item->position, game.position))
		{
			Item_Stack leftover;

			if (game_add_to_inventory(item->stack, &leftover))
			{
				log_debug("Added %d of item %d to inventory\n", item->stack.count, item->stack.type);
				item->stack = (Item_Stack) {.type = ITEM_NONE, .count = 0};
				item->position = v2(0.0f, 0.0f);
				item->player_detection_time = 0.0f;
			}
			else
			{
				item->stack = leftover;
			}
		}
	}
}

void game_render()
{
	ivec2	player_tile;

	gfx_set_projection(GFX_BUILTIN_SHADER_TEXTURE, mat4_world_to_ndc_projection(settings.zoom));
	gfx_set_projection(GFX_BUILTIN_SHADER_COLOUR, mat4_world_to_ndc_projection(settings.zoom));
	
	// Render world
	player_tile = vec2_round_ivec2(game.position);

	//  Tiles
	for (int y = MAP_SIZE_X-1; y >= 0; y--)
	{
		for (int x = 0; x < MAP_SIZE_X; x++)
		{
			Map_Tile *tile;
			vec2 from_player;

			tile = game_get_maptile(iv2(x, y ));

			from_player = vec2_sub_vec2(v2((float)x, (float)y), game.position);

			gfx_draw_quad(gfx_quad_tex_bl(from_player, v2(1.0f, 1.0f), TILE_INFO(game_get_top_tile_from_maptile(tile))->sprite));

			// Highlight

			if (ivec2_eq(iv2(x, y), game.mouse_tile) && game.current_ui == GAME_UI_NONE)
			{
				gfx_flush_sprite_quads();
				gfx_draw_quad(gfx_quad_colour_bl(from_player, v2(1.0f, 1.0f), v4(0.7f, 0.7f, 0.7f, 0.2f)));
				gfx_flush_colour_quads();
			}
		}
	}

	for (int i = 0; i < GAME_MAX_GROUND_ITEM_COUNT; i++)
	{
		Ground_Item *item;

		item = &game.ground_items[i];

		if (item->stack.type != ITEM_NONE && item->stack.count > 0)
			gfx_draw_quad(gfx_quad_tex_bl(vec2_sub_vec2(vec2_add_vec2(item->position, v2(0.1f, 0.1f)), game.position), v2(0.8f, 0.8f), ITEM_INFO(item->stack.type)->sprite));
	}

	// Ground objects (and player for proper layering)

	for (int y = MAP_SIZE_X-1; y >= 0; y--)
	{
		for (int x = 0; x < MAP_SIZE_X; x++)
		{
			Map_Tile *tile;
			Ground_Object_Info *info;
			vec2 from_player;

			tile = game_get_maptile(iv2(x, y));
			info = GROUND_OBJECT_INFO(tile->ground_object);

			from_player = vec2_sub_vec2(v2((float)x, (float)y), game.position);

			if (tile->ground_object != GROUND_OBJECT_NONE)
				gfx_draw_quad(gfx_quad_tex_bl(from_player, info->size, info->sprite));
		}

		if (y == player_tile.y)
		{
			gfx_draw_quad(gfx_quad_tex_centred(v2(0.0f, 0.0f), v2(0.8, 1.6f), reg.player_sprite));
		}
	}

	// Render UI

	gfx_flush_colour_quads();
	gfx_flush_sprite_quads();
	gfx_set_projection(GFX_BUILTIN_SHADER_TEXTURE, mat4_ui_projection(v2(1280.0f, 720.0f)));
	gfx_set_projection(GFX_BUILTIN_SHADER_COLOUR, mat4_ui_projection(v2(1280.0f, 720.0f)));

	// Hotbar
	
	for (int i = 0; i < 9; i++)
	{
		Item_Stack stack;

		if (game.current_hotbar_slot == i)
			gfx_draw_quad(gfx_quad_tex_bl(v2(UI_HOTBAR_POSITION_X + i * UI_INVENTORY_CELL_SIZE, 0.0f), v2(UI_INVENTORY_CELL_SIZE, UI_INVENTORY_CELL_SIZE), reg.ui_hotbar_active_sprite));
		else
			gfx_draw_quad(gfx_quad_tex_bl(v2(UI_HOTBAR_POSITION_X + i * UI_INVENTORY_CELL_SIZE, 0.0f), v2(UI_INVENTORY_CELL_SIZE, UI_INVENTORY_CELL_SIZE), reg.ui_hotbar_sprite));
		
		stack = game.inventory[i];

		if (stack.count > 0)
		{
			gfx_draw_quad(gfx_quad_tex_bl(v2(UI_HOTBAR_POSITION_X + i * UI_INVENTORY_CELL_SIZE + UI_INVENTORY_ITEM_OFFSET, UI_INVENTORY_ITEM_OFFSET), v2(UI_INVENTORY_ITEM_SIZE, UI_INVENTORY_ITEM_SIZE), reg.items[stack.type].sprite));
			game_ui_render_number(v2(UI_HOTBAR_POSITION_X + i * UI_INVENTORY_CELL_SIZE + UI_INVENTORY_ITEM_TEXT_OFFSET_X, UI_INVENTORY_ITEM_TEXT_OFFSET_Y), UI_INVENTORY_ITEM_TEXT_SIZE, stack.count);
		}
	}

	// Inventory

	if (game.current_ui == GAME_UI_INVENTORY)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int i = 0; i < 9; i++)
			{
				Item_Stack stack;

				if (game.mouse_inventory_index == j*9 + i)
					gfx_draw_quad(gfx_quad_tex_bl(v2(UI_INVENTORY_POSITION_X + i * UI_INVENTORY_CELL_SIZE, UI_INVENTORY_POSITION_Y + UI_INVENTORY_CELL_SIZE * j), v2(UI_INVENTORY_CELL_SIZE, UI_INVENTORY_CELL_SIZE), reg.ui_hotbar_active_sprite));
				else
					gfx_draw_quad(gfx_quad_tex_bl(v2(UI_INVENTORY_POSITION_X + i * UI_INVENTORY_CELL_SIZE, UI_INVENTORY_POSITION_Y + UI_INVENTORY_CELL_SIZE * j), v2(UI_INVENTORY_CELL_SIZE, UI_INVENTORY_CELL_SIZE), reg.ui_hotbar_sprite));
				
				stack = game.inventory[j*9 + i];

				if (stack.count > 0)
				{
					gfx_draw_quad(gfx_quad_tex_bl(v2(UI_INVENTORY_POSITION_X + i * UI_INVENTORY_CELL_SIZE + UI_INVENTORY_ITEM_OFFSET, UI_INVENTORY_POSITION_Y + UI_INVENTORY_ITEM_OFFSET + UI_INVENTORY_CELL_SIZE * j), v2(UI_INVENTORY_ITEM_SIZE, UI_INVENTORY_ITEM_SIZE), reg.items[stack.type].sprite));
					game_ui_render_number(v2(UI_INVENTORY_POSITION_X + i * UI_INVENTORY_CELL_SIZE + UI_INVENTORY_ITEM_TEXT_OFFSET_X, UI_INVENTORY_POSITION_Y + UI_INVENTORY_ITEM_TEXT_OFFSET_Y + UI_INVENTORY_CELL_SIZE * j), UI_INVENTORY_ITEM_TEXT_SIZE, stack.count);
				}
			}
		}

		if (game.inventory_hand.count > 0 && game.inventory_hand.type != ITEM_NONE)
		{
			gfx_draw_quad(gfx_quad_tex_centred(game.mouse_position_ui, v2(UI_INVENTORY_ITEM_SIZE, UI_INVENTORY_ITEM_SIZE), reg.items[game.inventory_hand.type].sprite));                           
			// game_ui_render_number(vec2_add_vec2(game.mouse_position_ui, v2(UI_INVENTORY_ITEM_TEXT_OFFSET_X, UI_INVENTORY_ITEM_TEXT_OFFSET_Y)), UI_INVENTORY_ITEM_TEXT_SIZE, game.inventory_hand.count);
		}
	}
}

void game_ui_render_number(vec2 position, float number_size, int number)
{
	int index = 0;

	while (number > 0)
	{
		int digit;

		digit = number % 10;
		number = number / 10;

		gfx_draw_quad(gfx_quad_tex_bl(vec2_sub_vec2(position, v2(number_size * index, 0.0f)), v2(number_size, number_size), reg.number_sprites[digit]));

		index++;
	}
}

Map_Tile* game_get_maptile(ivec2 index)
{
	if (index.x < 0 || index.x >= MAP_SIZE_X || index.y < 0 || index.y >= MAP_SIZE_X)
	{
		log_warning("Accessing tile (%d, %d) which is outside the map\n", index.x, index.y);
		return NULL;
	}

	return &game.map.tiles[index.y * MAP_SIZE_X + index.x];
}

Tile_Type game_get_top_tile(ivec2 index)
{
	Map_Tile *tile;

	tile = game_get_maptile(index);
	if (!tile)
	{
		log_warning("Getting top of tile (%d, %d) which is outside the map\n", index.x, index.y);
		return TILE_NONE;
	}

	if (tile->foreground != TILE_NONE) return tile->foreground;

	return tile->background;
}

Tile_Type game_get_top_tile_from_maptile(Map_Tile *tile)
{
	if (tile->foreground != TILE_NONE) return tile->foreground;
	return tile->background;
}

int game_add_ground_item(Ground_Item *item)
{
	if (game.ground_item_count >= GAME_MAX_GROUND_ITEM_COUNT)
	{
		log_warning("Trying to add ground item, but there's no space");
		return 0;
	}
	
	// Find the next available slot
	
	for (int i = 0; i < GAME_MAX_GROUND_ITEM_COUNT; i++)
	{
		Ground_Item *current_item;

		current_item = &game.ground_items[game.ground_item_index + i];

		if (current_item->stack.type == ITEM_NONE || current_item->stack.count == 0)
		{
			*current_item = *item;
			game.ground_item_index += i;
			game.ground_item_count++;
			return 1;
		}
	}

	log_error("Couldn't find a free slot for ground item even though there should be space");
	return 0;
}

Drop_Stack game_droptable(Droptable_Type type)
{
	Drop_Stack drop;
	
	switch(type)
	{
		case DROPTABLE_NONE:
			drop.stack_count = 0;
		break;

		case DROPTABLE_TREE:
			drop.stack_count = 1;
			drop.stacks[0].type = ITEM_WOOD;
			drop.stacks[0].count = 5;
		break;
		
		case DROPTABLE_STONE:
			drop.stack_count = 1;
			drop.stacks[0].type = ITEM_STONE;
			drop.stacks[0].count = 3;
		break;

		default:
			log_warning("Trying to roll droptable %d which doesn't exist\n", type);
		break;
	}

	return drop;
}

Item_Stack *game_get_handitem()
{
	return &game.inventory[game.current_hotbar_slot];
}

int game_place_item(ivec2 tile_pos, Item_Type type)
{
	Item_Info *info;
	Map_Tile  *tile;

	info = ITEM_INFO(type);
	tile = game_get_maptile(tile_pos);
	
	if (!tile)
		return 0;

	if (info->placeable == PLACEABLE_NOT)
		return 0;

	switch(info->placeable)
	{
		case PLACEABLE_FOREGROUND:
			if (tile->foreground != TILE_NONE)
				return 0;
			tile->foreground = (Tile_Type) info->place_type;
		break;

		case PLACEABLE_GROUND_OBJECT:
			if (tile->ground_object != GROUND_OBJECT_NONE)
				return 0;
			tile->ground_object = (Ground_Object_Type) info->place_type;
		break;

		case PLACEABLE_NOT:
			log_warning("Trying to place an item which isn't placeable\n");
			return 0;
		break;

		default:
			log_warning("Trying to place an item with an unknown placeable flag\n");
			return 0;
		break;
	}

	return 1;
}

int game_add_to_inventory(Item_Stack stack, Item_Stack *out_leftover)
{
	Item_Stack *empty_stack;

	out_leftover->type = ITEM_NONE;
	out_leftover->count = 0;
	empty_stack = NULL;

	for (int i = 0; i < 36; i++)
	{
		Item_Stack *current_stack;

		current_stack = &game.inventory[i];

		if (current_stack->type == stack.type)
		{
			int amount_can_add;
			int amount_to_add;

			amount_can_add = ITEM_INFO(current_stack->type)->max_stack - current_stack->count;
			
			amount_to_add = stack.count;
			if (amount_to_add > amount_can_add)
				amount_to_add = amount_can_add;

			current_stack->count += amount_to_add;
			stack.count -= amount_to_add;

			if (stack.count <= 0)
			{
				return 1;
			}
		}

		else if (current_stack->type == ITEM_NONE && !empty_stack)
			empty_stack = current_stack;
	}

	if (empty_stack)
	{
		*empty_stack = stack;
		return 1;
	}

	*out_leftover = stack;

	return 0;
}

