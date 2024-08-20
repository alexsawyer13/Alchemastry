#ifndef _GFX_H_
#define _GFX_H_

#include "maths.h"

#define GFX_MAX_QUADS 1024
#define GFX_MAX_SPRITES 1024

typedef struct
{
	int		max_texture_units;
} Platform_Info;

typedef enum
{
	GFX_ORIGIN_CENTRE,
	GFX_ORIGIN_BOTTOM_LEFT,
} Gfx_Origin;

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
	vec2		position;
	vec2		size;
	float		angle;
	Gfx_Origin	origin;

	vec4		colour;
} ColouredQuad;

typedef struct
{
	vec2		position;
	vec2		size;
	float		angle;
	Gfx_Origin	origin;

	Sprite		sprite;
} TexturedQuad;

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

	ColouredQuad	quads[GFX_MAX_QUADS];
	int				quad_count;
	Mesh			quad_mesh;
	float			*quad_buffer;

	Sprite			sprites[GFX_MAX_SPRITES];
	int				sprite_count;
	Mesh			sprite_mesh;
	float			*sprite_buffer;
} Gfx;


int		gfx_init();
void	gfx_shutdown();

void	gfx_print_platform_info();

void	gfx_mesh_destroy(Mesh *mesh);

void	gfx_start_frame(vec4 bg_colour);
void	gfx_end_frame();

void	gfx_draw_quad(ColouredQuad *quad);
void	gfx_flush_quads();

void	gfx_draw_sprite(Sprite *sprite);
void	gfx_flush_sprites();

int		gfx_shader_create(Shader *shader, const char *vertex_path, const char *fragment_path);
void	gfx_shader_destroy(Shader *shader);
void	gfx_shader_unform_mat4(Shader *shader, mat4 mat, const char *uniform_name);

#endif