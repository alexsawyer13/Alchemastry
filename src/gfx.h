#ifndef _GFX_H_
#define _GFX_H_

#include "maths.h"

#define GFX_MAX_COLOUR_QUADS 1024
#define GFX_MAX_SPRITE_QUADS 1024

typedef struct
{
	int		max_texture_units;
} Platform_Info;

typedef enum
{
	GFX_ORIGIN_CENTRE,
	GFX_ORIGIN_BOTTOM_LEFT,
} Gfx_Origin;

typedef enum
{
	GFX_QUAD_COLOUR,
	GFX_QUAD_SPRITE
} Gfx_Quad_Type;

typedef struct
{
	int			id;
} Texture;

typedef struct
{
	Texture		*texture;
	vec2		tex_coord_bottom_left;
	vec2		tex_coord_size;
} Sprite;

typedef struct
{
	vec2			position;
	vec2			size;
	float			angle;
	Gfx_Origin		origin;

	Gfx_Quad_Type 	type;
	union
	{
		vec4		colour;
		Sprite		sprite;
	};
	
} Quad;

typedef struct
{
	unsigned int	vao;
	unsigned int	vbo;
	unsigned int	ibo;
} Mesh;

typedef struct
{
	unsigned int	id;
} Shader;

typedef struct
{
	Platform_Info	info;
	ivec2			viewport;

	Shader			colour_shader;
	Shader			texture_shader;

	Quad			colour_quads[GFX_MAX_COLOUR_QUADS];
	int				colour_quad_count;
	Mesh			colour_quad_mesh;
	float			*colour_quad_buffer;

	Quad			sprite_quads[GFX_MAX_SPRITE_QUADS];
	int				sprite_quad_count;
	Mesh			sprite_quad_mesh;
	float			*sprite_quad_buffer;
} Gfx;

int		gfx_init();
void	gfx_shutdown();

void	gfx_print_platform_info();

void	gfx_mesh_destroy(Mesh *mesh);

void	gfx_start_frame(vec4 bg_colour);
void	gfx_end_frame();

Quad	gfx_quad_colour_centred(vec2 position, vec2 size, vec4 colour, float angle);

void	gfx_draw_quad(Quad quad);
void	gfx_flush_colour_quads();
void	gfx_flush_sprite_quads();

int		gfx_shader_create(Shader *shader, const char *vertex_path, const char *fragment_path);
void	gfx_shader_destroy(Shader *shader);
void	gfx_shader_unform_mat4(Shader *shader, mat4 mat, const char *uniform_name);

#endif