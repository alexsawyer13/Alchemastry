#ifndef _GAME_H_
#define _GAME_H_

#include "maths.h"
#include "gfx.h"

#define MAP_SIZE_X 256
#define MAP_SIZE_A 65536

#define UI_HOTBAR_POSITION_X 370.0f
#define UI_HOTBAR_POSITION_Y 0.0f
#define UI_INVENTORY_POSITION_X 370.0f
#define UI_INVENTORY_POSITION_Y 150.0f
#define	UI_INVENTORY_CELL_SIZE 60.0f
#define UI_INVENTORY_ITEM_OFFSET 10.0f
#define UI_INVENTORY_ITEM_SIZE 40.0f
#define UI_INVENTORY_ITEM_TEXT_OFFSET_X 38.0f
#define UI_INVENTORY_ITEM_TEXT_OFFSET_Y 10.0f
#define UI_INVENTORY_ITEM_TEXT_SIZE 15.0f

#define GAME_MAX_GROUND_ITEM_COUNT 1024

typedef struct
{
	Sprite 	sprite;
	float 	speed_multiplier;
} Tile_Info;

typedef enum
{
	TILE_NONE,
	TILE_DIRT,
	TILE_GRASS,
	TILE_STONE,
	TILE_PATH,

	TILE_COUNT
} Tile_Type;

typedef struct
{
	Sprite 	sprite;
	vec2	size;
} Foreground_Info;

typedef enum
{
	FOREGROUND_NONE,
	FOREGROUND_TREE,
	FOREGROUND_ROCK,

	FOREGROUND_COUNT
} Foreground_Type;

typedef struct
{
	Tile_Type 			type;
	Foreground_Type 	foreground;
} Tile;

typedef struct
{
	Tile 		tiles[MAP_SIZE_A];
} Map;

typedef enum
{
	ITEM_NONE,
	ITEM_WOOD,
	ITEM_STONE,
	ITEM_ELISHA,

	ITEM_COUNT
} Item_Type;

typedef struct
{
	int		max_stack;

	Sprite	sprite;
} Item_Info;

typedef struct
{
	Item_Type	type;
	int			count;
} Item_Stack;

typedef struct
{
	vec2		position;
	Item_Stack	stack;
} Ground_Item;

typedef enum
{
	GAME_UI_NONE,
	GAME_UI_INVENTORY,
} Ui_Panel_Type;

typedef struct
{
	float			zoom;
} Settings;

typedef struct
{
	TextureAtlas 	atlas;
	Sprite			player_sprite;

	Texture			elisha_texture;
	Sprite			elisha_sprite;

	Sprite			number_sprites[10];

	Tile_Info 		tiles[TILE_COUNT];
	Foreground_Info	foregrounds[FOREGROUND_COUNT];
	Item_Info		items[ITEM_COUNT];

	Sprite			ui_hotbar_sprite;
	Sprite			ui_hotbar_active_sprite;
} Registry;

typedef struct
{
	Map			map;

	vec2		position;
	float		speed;

	Item_Stack	inventory[36];
	Item_Stack	inventory_hand;
	int			current_hotbar_slot;

	Ground_Item	ground_items[GAME_MAX_GROUND_ITEM_COUNT];
	int			ground_item_count;

	vec2		mouse_position_world;
	vec2		mouse_position_ui;
	int			mouse_tile_index;
	int			mouse_inventory_index;

	Ui_Panel_Type	current_ui;
} Game;

int		game_init();
void	game_shutdown();

void	game_update();
void	game_render();

void	game_ui_render_number(vec2 position, float number_size, int number);

void	registry_init();

#endif