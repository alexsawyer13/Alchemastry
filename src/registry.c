#include "game.h"

#include <stdio.h>

extern Registry reg;

void registry_init()
{
	gfx_texture_create(&reg.elisha_texture, "assets/textures/Elisha.png");
	reg.elisha_sprite.texture = reg.elisha_texture;
	reg.elisha_sprite.tex_coord_origin = v2(0.0f, 0.0f);
	reg.elisha_sprite.tex_coord_size = v2(1.0f, 1.0f);

	gfx_texture_atlas_create(&reg.atlas, "assets/textures/atlas.png", iv2(8, 8));

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

	reg.items[ITEM_NONE] = (ItemInfo){
		.max_stack = 0,
		.sprite = gfx_texture_atlas_sprite(&reg.atlas, iv2(0, 0), iv2(1, 1))
	};

	reg.items[ITEM_WOOD] = (ItemInfo){
		.max_stack = 999,
		.sprite = gfx_texture_atlas_sprite(&reg.atlas, iv2(4, 1), iv2(1, 1))
	};

	reg.items[ITEM_STONE] = (ItemInfo){
		.max_stack = 999,
		.sprite = gfx_texture_atlas_sprite(&reg.atlas, iv2(5, 1), iv2(1, 1))
	};

	reg.items[ITEM_ELISHA] = (ItemInfo){
		.max_stack = 1,
		.sprite = reg.elisha_sprite
	};

	for (int i = 0; i < 10; i++)
	{
		reg.number_sprites[i] = gfx_texture_atlas_sprite(&reg.atlas, iv2(5 + i, 0), iv2(1, 1));
	}

	reg.ui_hotbar_sprite = gfx_texture_atlas_sprite(&reg.atlas, iv2(3, 1), iv2(1, 1));
	reg.ui_hotbar_active_sprite = gfx_texture_atlas_sprite(&reg.atlas, iv2(3, 2), iv2(1, 1));

}