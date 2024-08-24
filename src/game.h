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
	TileType 	type;
} Tile;

typedef struct
{
	Tile 		tiles[MAP_SIZE_A];
} Map;

typedef struct
{
	TileInfo 	tiles[TILE_COUNT];
	Texture 	tilemap;
} Registry;

typedef struct
{
	Map			map;

	vec2		position;
	float		speed;
} Game;

int		game_init();
void	game_shutdown();

void	game_update();
void	game_render();

void	registry_init();

#endif