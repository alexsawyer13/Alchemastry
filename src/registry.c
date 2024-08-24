#include "game.h"

#include <stdio.h>

extern Registry reg;

void registry_init()
{
	gfx_texture_atlas_create(&reg.atlas, "assets/tiles/atlas.png", iv2(8, 8));

	reg.player_sprite = gfx_texture_atlas_sprite(&reg.atlas, iv2(2, 1), iv2(1, 2));

	reg.tiles[TILE_NONE] = (TileInfo){
		.speed_multiplier = 1.0f,
		.sprite = gfx_texture_atlas_sprite(&reg.atlas, iv2(0, 0), iv2(1, 1))
	};

	reg.tiles[TILE_GRASS] = (TileInfo){
		.speed_multiplier = 1.0,
		.sprite = gfx_texture_atlas_sprite(&reg.atlas, iv2(1, 0), iv2(1, 1))
	};

	reg.tiles[TILE_DIRT] = (TileInfo){
		.speed_multiplier = 1.0,
		.sprite = gfx_texture_atlas_sprite(&reg.atlas, iv2(2, 0), iv2(1, 1))
	};

	reg.tiles[TILE_STONE] = (TileInfo){
		.speed_multiplier = 1.0,
		.sprite = gfx_texture_atlas_sprite(&reg.atlas, iv2(3, 0), iv2(1, 1))
	};

	reg.tiles[TILE_PATH] = (TileInfo){
		.speed_multiplier = 2.0,
		.sprite = gfx_texture_atlas_sprite(&reg.atlas, iv2(4, 0), iv2(1, 1))
	};

	reg.foregrounds[FOREGROUND_NONE] = (ForegroundInfo){
		.sprite = gfx_texture_atlas_sprite(&reg.atlas, iv2(0, 0), iv2(1, 1)),
		.size = v2(1.0f, 1.0f)
	};

	reg.foregrounds[FOREGROUND_TREE] = (ForegroundInfo){
		.sprite = gfx_texture_atlas_sprite(&reg.atlas, iv2(0, 1), iv2(1, 2)),
		.size = v2(1.0f, 2.0f)
	};

	reg.foregrounds[FOREGROUND_ROCK] = (ForegroundInfo){
		.sprite = gfx_texture_atlas_sprite(&reg.atlas, iv2(1, 1), iv2(1, 1)),
		.size = v2(1.0f, 1.0f)
	};

}