#ifndef _GAME_H_
#define _GAME_H_

#include "maths.h"
#include "gfx.h"

#define MAP_SIZE_X 256
#define MAP_SIZE_A 65536

#define UI_HOTBAR_POSITION_X 370.0f
#define UI_HOTBAR_POSITION_Y 0.0f
#define UI_INVENTORY_POSITION_X 370.0f
#define UI_INVENTORY_POSITION_Y 200.0f
#define	UI_INVENTORY_CELL_SIZE 60.0f
#define UI_INVENTORY_ITEM_OFFSET 10.0f
#define UI_INVENTORY_ITEM_SIZE 40.0f

typedef struct
{
	Sprite 	sprite;
	float 	speed_multiplier;
} TileInfo;

typedef enum
{
	TILE_NONE,
	TILE_DIRT,
	TILE_GRASS,
	TILE_STONE,
	TILE_PATH,

	TILE_COUNT
} TileType;

typedef struct
{
	Sprite 	sprite;
	vec2	size;
} ForegroundInfo;

typedef enum
{
	FOREGROUND_NONE,
	FOREGROUND_TREE,
	FOREGROUND_ROCK,

	FOREGROUND_COUNT
} ForegroundType;

typedef struct
{
	TileType 		type;
	ForegroundType 	foreground;
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
} ItemType;

typedef struct
{
	int		max_stack;

	Sprite	sprite;
} ItemInfo;

typedef struct
{
	ItemType	type;
	int			count;
} ItemStack;

typedef enum
{
	GAME_UI_NONE,
	GAME_UI_INVENTORY,
} UiPanels;

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

	TileInfo 		tiles[TILE_COUNT];
	ForegroundInfo	foregrounds[FOREGROUND_COUNT];
	ItemInfo		items[ITEM_COUNT];

	Sprite			ui_hotbar_sprite;
	Sprite			ui_hotbar_active_sprite;
} Registry;

typedef struct
{
	Map			map;

	vec2		position;
	float		speed;

	ItemStack	inventory[36];
	ItemStack	inventory_hand;
	int			current_hotbar_slot;

	vec2		mouse_position_world;
	vec2		mouse_position_ui;
	int			mouse_tile_index;
	int			mouse_inventory_index;

	UiPanels	current_ui;
} Game;

int		game_init();
void	game_shutdown();

void	game_update();
void	game_render();

void	registry_init();

#endif