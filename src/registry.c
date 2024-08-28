#include "game.h"

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

	reg.ground_objects[GROUND_OBJECT_NONE] = (Ground_Object_Info){
		.droptable = DROPTABLE_NONE,
		.sprite = gfx_texture_atlas_sprite(&reg.atlas, iv2(0, 0), iv2(1, 1)),
		.size = v2(1.0f, 1.0f)
	};

	reg.ground_objects[GROUND_OBJECT_TREE] = (Ground_Object_Info){
		.droptable = DROPTABLE_TREE,
		.sprite = gfx_texture_atlas_sprite(&reg.atlas, iv2(0, 1), iv2(1, 2)),
		.size = v2(1.0f, 2.0f)
	};

	reg.ground_objects[GROUND_OBJECT_ROCK] = (Ground_Object_Info){
		.droptable = DROPTABLE_STONE,
		.sprite = gfx_texture_atlas_sprite(&reg.atlas, iv2(1, 1), iv2(1, 1)),
		.size = v2(1.0f, 1.0f)
	};

	reg.items[ITEM_NONE] = (Item_Info){
		.max_stack = 0,
		.placeable = PLACEABLE_NOT,
		.place_type = 0,
		.sprite = gfx_texture_atlas_sprite(&reg.atlas, iv2(0, 0), iv2(1, 1))
	};

	reg.items[ITEM_WOOD] = (Item_Info){
		.max_stack = 999,
		.placeable = PLACEABLE_NOT,
		.place_type = 0,
		.sprite = gfx_texture_atlas_sprite(&reg.atlas, iv2(4, 1), iv2(1, 1))
	};

	reg.items[ITEM_STONE] = (Item_Info){
		.max_stack = 999,
		.placeable = PLACEABLE_NOT,
		.place_type = 0,
		.sprite = gfx_texture_atlas_sprite(&reg.atlas, iv2(5, 1), iv2(1, 1))
	};

	reg.items[ITEM_ELISHA] = (Item_Info){
		.max_stack = 1,
		.placeable = PLACEABLE_NOT,
		.place_type = 0,
		.sprite = reg.elisha_sprite
	};

	reg.items[ITEM_PATH] = (Item_Info){
		.max_stack = 999,
		.placeable = PLACEABLE_FOREGROUND,
		.place_type = TILE_PATH,
		.sprite = gfx_texture_atlas_sprite(&reg.atlas, iv2(4, 0), iv2(1, 1))
	};

	for (int i = 0; i < 10; i++)
	{
		reg.number_sprites[i] = gfx_texture_atlas_sprite(&reg.atlas, iv2(5 + i, 0), iv2(1, 1));
	}

	reg.ui_hotbar_sprite = gfx_texture_atlas_sprite(&reg.atlas, iv2(3, 1), iv2(1, 1));
	reg.ui_hotbar_active_sprite = gfx_texture_atlas_sprite(&reg.atlas, iv2(3, 2), iv2(1, 1));
};

void registry_shutdown()
{
}

Tile_Info* TILE_INFO(Tile_Type type)
{
	return &reg.tiles[type];
}

Ground_Object_Info* GROUND_OBJECT_INFO(Ground_Object_Type type)
{
	return &reg.ground_objects[type];
}

Item_Info* ITEM_INFO(Item_Type type)
{
	return &reg.items[type];
}
