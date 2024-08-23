#ifndef _GAME_H_
#define _GAME_H_

#include "maths.h"
#include "gfx.h"

typedef enum
{
	GAME_ENEMY_TEST
} Game_Enemy_Type;

typedef struct
{
	Game_Enemy_Type		type;
	vec2				position;
	int					health;
} Enemy;

typedef struct
{
	Sprite 	background;
	vec2 	path[32];
} Map;

typedef struct
{
	Map		maps[32];
} Registry;

typedef struct
{
	Map		current_map;
} Game;

int		game_init();
void	game_shutdown();

void	game_update();
void	game_render();

#endif