#ifndef _GFX_H_
#define _GFX_H_

#include "core/maths.h"

#define GFX_MAX_COLOUR_QUADS 10240
#define GFX_MAX_SPRITE_QUADS 10240

typedef struct
{
	int      max_texture_units;
} Platform_Info;

typedef enum
{
	GFX_QUAD_TYPE_COLOUR,
	GFX_QUAD_TYPE_SPRITE
} Gfx_Quad_Type;

typedef struct
{
	int     id;
	int     width;
	int     height;
	int     channels;
} Texture;

typedef struct
{
	Texture     texture;
	ivec2       cell_size_pixels;
	vec2        cell_size_tex;

	float       delta;
	float       two_delta;
} TextureAtlas;

typedef struct
{
	Texture		texture;
	vec2		tex_coord_origin;
	vec2		tex_coord_size;
} Sprite;

typedef struct
{
	Quad            quad;
	Gfx_Quad_Type   type;
	union
	{
		vec4        colour;
		Sprite      sprite;
	};
	
} Gfx_Quad;

typedef struct
{
	unsigned int    vao;
	unsigned int    vbo;
	unsigned int    ibo;
} Mesh;

typedef enum
{
	GFX_BUILTIN_SHADER_COLOUR,
	GFX_BUILTIN_SHADER_TEXTURE
} Gfx_Builtin_Shader;

typedef struct
{
	unsigned int	id;
} Shader;

typedef struct
{
	Platform_Info	info;
	ivec2		viewport;

	Shader		colour_shader;
	Shader		texture_shader;

	Gfx_Quad	colour_quads[GFX_MAX_COLOUR_QUADS];
	int		colour_quad_count;
	Mesh		colour_quad_mesh;
	float		*colour_quad_buffer;

	Gfx_Quad	sprite_quads[GFX_MAX_SPRITE_QUADS];
	int		sprite_quad_count;
	Mesh		sprite_quad_mesh;
	float		*sprite_quad_buffer;
} Gfx;

int		gfx_init();
void		gfx_shutdown();

void		gfx_opengl_error_callback(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam);

void		gfx_print_platform_info();

void		gfx_mesh_destroy(Mesh *mesh);

void		gfx_start_frame(vec4 bg_colour);
void		gfx_end_frame();

Gfx_Quad	gfx_quad_colour_centred(vec2 position, vec2 size, vec4 colour);
Gfx_Quad	gfx_quad_colour_bl(vec2 position, vec2 size, vec4 colour);
Gfx_Quad	gfx_quad_tex_bl(vec2 position, vec2 size, Sprite sprite);
Gfx_Quad	gfx_quad_tex_centred(vec2 position, vec2 size, Sprite sprite);

void		gfx_draw_quad(Gfx_Quad quad);
void		gfx_flush_colour_quads();
void		gfx_flush_sprite_quads();

int		gfx_shader_create_from_source(Shader *out_shader, const char *vertex_source, const char *fragment_source);
int		gfx_shader_create_from_file(Shader *out_shader, const char *vertex_path, const char *fragment_path);
void		gfx_shader_destroy(Shader *shader);
void		gfx_shader_uniform_mat4(Shader *shader, mat4 mat, const char *uniform_name);
void		gfx_shader_uniform_int(Shader *shader, int value, const char *uniform_name);
void		gfx_shader_uniform_int_array(Shader *shader, int count, int *values, const char *uniform_name);

void		gfx_set_projection(Gfx_Builtin_Shader shader, mat4 projection);

int		gfx_texture_create(Texture *out_texture, const char *texture_path);
void		gfx_texture_bind(Texture *texture, int texture_unit);

int		gfx_texture_atlas_create(TextureAtlas *atlas, const char *path, ivec2 cell_size_pixels);
Sprite		gfx_texture_atlas_sprite(TextureAtlas *atlas, ivec2 cell_coord, ivec2 cell_size);

#endif
