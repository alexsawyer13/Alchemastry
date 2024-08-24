#ifndef _GAME_H_
#define _GAME_H_

#include "maths.h"
#include "gfx.h"

#define MAP_SIZE_X 256
#define MAP_SIZE_A 65536

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

typedef struct
{
	TextureAtlas 	atlas;
	Sprite			player_sprite;

	TileInfo 		tiles[TILE_COUNT];
	ForegroundInfo	foregrounds[FOREGROUND_COUNT];
} Registry;

typedef struct
{
	float			zoom;
} Settings;

typedef struct
{
	Map			map;

	vec2		position;
	float		speed;

	vec2		mouse_position_world;
	int			mouse_tile_index;
} Game;

int		game_init();
void	game_shutdown();

void	game_update();
void	game_render();

void	registry_init();

#endif