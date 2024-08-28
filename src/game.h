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
#define DROP_MAX_ITEMSTACKS 16

typedef enum
{
	DROPTABLE_NONE,
	DROPTABLE_TREE,
	DROPTABLE_STONE,
} Droptable_Type;

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
	Droptable_Type	droptable;

	Sprite		sprite;
	vec2		size;
} Ground_Object_Info;

typedef enum
{
	GROUND_OBJECT_NONE,
	GROUND_OBJECT_TREE,
	GROUND_OBJECT_ROCK,

	GROUND_OBJECT_COUNT
} Ground_Object_Type;

typedef struct
{
	Tile_Type 		background;
	Tile_Type		foreground;
	Ground_Object_Type	ground_object;
} Map_Tile;

typedef struct
{
	Map_Tile 		tiles[MAP_SIZE_A];
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
	int	max_stack;

	Sprite	sprite;
} Item_Info;

typedef struct
{
	Item_Type	type;
	int		count;
} Item_Stack;

typedef struct
{
	Item_Stack	stacks[DROP_MAX_ITEMSTACKS];
	int		stack_count;
} Drop_Stack;

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
	TextureAtlas		atlas;
	Sprite			player_sprite;

	Texture			elisha_texture;
	Sprite			elisha_sprite;

	Sprite			number_sprites[10];

	Tile_Info 		tiles[TILE_COUNT];
	Ground_Object_Info	ground_objects[GROUND_OBJECT_COUNT];
	Item_Info		items[ITEM_COUNT];

	Sprite			ui_hotbar_sprite;
	Sprite			ui_hotbar_active_sprite;
} Registry;

typedef struct
{
	Map		map;

	vec2		position;
	float		speed;

	Item_Stack	inventory[36];
	Item_Stack	inventory_hand;
	int		current_hotbar_slot;

	Ground_Item	ground_items[GAME_MAX_GROUND_ITEM_COUNT];
	int		ground_item_count;
	int		ground_item_index;

	vec2		mouse_position_world;
	vec2		mouse_position_ui;

	ivec2		mouse_tile;
	int		mouse_tile_index;
	int		mouse_inventory_index;

	Ui_Panel_Type	current_ui;

} Game;

int		game_init();
void		game_shutdown();

void		game_update();
void		game_render();

void		game_ui_render_number(vec2 position, float number_size, int number);

int		game_add_ground_item(Ground_Item *item);

Map_Tile*	game_get_maptile(ivec2 index);
Tile_Type	game_get_top_tile(ivec2 index);

Tile_Type	game_get_top_tile_from_maptile(Map_Tile *tile);

Drop_Stack	game_droptable(Droptable_Type type);

// ---------- REGISTRY ----------

void			registry_init();
void			registry_shutdown();

Tile_Info*		TILE_INFO(Tile_Type type);
Ground_Object_Info*	GROUND_OBJECT_INFO(Ground_Object_Type type);
Item_Info*		ITEM_INFO(Item_Type type);

#endif
