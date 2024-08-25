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

	reg.tiles[TILE_NONE] = (Tile_Info){
		.speed_multiplier = 1.0f,
		.sprite = gfx_texture_atlas_sprite(&reg.atlas, iv2(0, 0), iv2(1, 1))
	};

	reg.tiles[TILE_GRASS] = (Tile_Info){
		.speed_multiplier = 1.0,
		.sprite = gfx_texture_atlas_sprite(&reg.atlas, iv2(1, 0), iv2(1, 1))
	};

	reg.tiles[TILE_DIRT] = (Tile_Info){
		.speed_multiplier = 1.0,
		.sprite = gfx_texture_atlas_sprite(&reg.atlas, iv2(2, 0), iv2(1, 1))
	};

	reg.tiles[TILE_STONE] = (Tile_Info){
		.speed_multiplier = 1.0,
		.sprite = gfx_texture_atlas_sprite(&reg.atlas, iv2(3, 0), iv2(1, 1))
	};

	reg.tiles[TILE_PATH] = (Tile_Info){
		.speed_multiplier = 2.0,
		.sprite = gfx_texture_atlas_sprite(&reg.atlas, iv2(4, 0), iv2(1, 1))
	};

	reg.foregrounds[FOREGROUND_NONE] = (Foreground_Info){
		.sprite = gfx_texture_atlas_sprite(&reg.atlas, iv2(0, 0), iv2(1, 1)),
		.size = v2(1.0f, 1.0f)
	};

	reg.foregrounds[FOREGROUND_TREE] = (Foreground_Info){
		.sprite = gfx_texture_atlas_sprite(&reg.atlas, iv2(0, 1), iv2(1, 2)),
		.size = v2(1.0f, 2.0f)
	};

	reg.foregrounds[FOREGROUND_ROCK] = (Foreground_Info){
		.sprite = gfx_texture_atlas_sprite(&reg.atlas, iv2(1, 1), iv2(1, 1)),
		.size = v2(1.0f, 1.0f)
	};

	reg.items[ITEM_NONE] = (Item_Info){
		.max_stack = 0,
		.sprite = gfx_texture_atlas_sprite(&reg.atlas, iv2(0, 0), iv2(1, 1))
	};

	reg.items[ITEM_WOOD] = (Item_Info){
		.max_stack = 999,
		.sprite = gfx_texture_atlas_sprite(&reg.atlas, iv2(4, 1), iv2(1, 1))
	};

	reg.items[ITEM_STONE] = (Item_Info){
		.max_stack = 999,
		.sprite = gfx_texture_atlas_sprite(&reg.atlas, iv2(5, 1), iv2(1, 1))
	};

	reg.items[ITEM_ELISHA] = (Item_Info){
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