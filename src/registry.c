#include "game.h"

extern Registry reg;

void registry_init()
{
	gfx_texture_create(&reg.tilemap, "assets/tiles/tilemap.png");

	reg.tiles[TILE_GRASS].sprite.texture = reg.tilemap;
	reg.tiles[TILE_GRASS].sprite.tex_coord_origin = v2(0.00390625, 0.00390625);
	reg.tiles[TILE_GRASS].sprite.tex_coord_size = v2(0.02734375, 0.02734375);

	reg.tiles[TILE_DIRT].sprite.texture = reg.tilemap;
	reg.tiles[TILE_DIRT].sprite.tex_coord_origin = v2(0.03515625, 0.00390625);
	reg.tiles[TILE_DIRT].sprite.tex_coord_size = v2(0.02734375, 0.02734375);
}