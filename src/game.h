#ifndef _GAME_H_
#define _GAME_H_

#include "maths.h"
#include "gfx.h"

typedef struct
{
	int tmp;
} Game;

int		game_init();
void	game_shutdown();

void	game_update();
void	game_render();

#endif