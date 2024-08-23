#include "gfx.h"
#include "platform.h"

#include <glad/glad.h>
#include <stb/stb_image.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

Gfx gfx;

int gfx_init()
{
	unsigned int *indices;

	// Load OpenGL function pointers
	gladLoadGLLoader(platform_get_gladloadproc());
	
	// Setup OpenGL error callback
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(gfx_opengl_error_callback, NULL);

	// Get the gfx platform info
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, (GLint *) & gfx.info.max_texture_units);

	// Load shaders
	if (!gfx_shader_create(&gfx.colour_shader, "assets/shaders/colour.vert", "assets/shaders/colour.frag"))
	{
		fprintf(stderr, "Failed to create colour shader\n");
		return 0;
	}

	if (!gfx_shader_create(&gfx.texture_shader, "assets/shaders/texture.vert", "assets/shaders/texture.frag"))
	{
		fprintf(stderr, "Failed to create texture shader\n");
		return 0;
	}

	// Colour quad rendering
	gfx.colour_quad_count = 0;

	glCreateVertexArrays(1, &gfx.colour_quad_mesh.vao);
	glGenBuffers(1, &gfx.colour_quad_mesh.vbo);
	glGenBuffers(1, &gfx.colour_quad_mesh.ibo);

	glBindVertexArray(gfx.colour_quad_mesh.vao);

	// Vertex buffer

	glBindBuffer(GL_ARRAY_BUFFER, gfx.colour_quad_mesh.vbo);
	glBufferData(GL_ARRAY_BUFFER, GFX_MAX_COLOUR_QUADS * 4 * 5 * sizeof(float), NULL, GL_DYNAMIC_DRAW);

	gfx.colour_quad_buffer = malloc(GFX_MAX_COLOUR_QUADS * 4 * 5 * sizeof(float));

	if (!gfx.colour_quad_buffer)
	{
		fprintf(stderr, "Failed to allocate memory for quad buffer\n");
		return 0;
	}

	// Index buffer

	indices = malloc(GFX_MAX_COLOUR_QUADS * 6 * sizeof(unsigned int));
	if (!indices)
	{
		fprintf(stderr, "Failed to allocate memory for colour quad indices");
		return 0;
	}

	for (int i = 0; i < GFX_MAX_COLOUR_QUADS; i++)
	{
		indices[i * 6 + 0] = i * 4 + 0;
		indices[i * 6 + 1] = i * 4 + 1;
		indices[i * 6 + 2] = i * 4 + 2;
		indices[i * 6 + 3] = i * 4 + 2;
		indices[i * 6 + 4] = i * 4 + 1;
		indices[i * 6 + 5] = i * 4 + 3;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gfx.colour_quad_mesh.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, GFX_MAX_COLOUR_QUADS * 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	free(indices);

	// a_Pos
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);

	// a_Colour
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)(2 * sizeof(float)));

	glBindVertexArray(0);

	// Sprite quad rendering
	gfx.sprite_quad_count = 0;
	
	glGenVertexArrays(1, &gfx.sprite_quad_mesh.vao);
	glBindVertexArray(gfx.sprite_quad_mesh.vao);
	glGenBuffers(1, &gfx.sprite_quad_mesh.vbo);
	glGenBuffers(1, &gfx.sprite_quad_mesh.ibo);

	// Vertex buffer

	glBindBuffer(GL_ARRAY_BUFFER, gfx.sprite_quad_mesh.vbo);
	glBufferData(GL_ARRAY_BUFFER, GFX_MAX_SPRITE_QUADS * 4 * 4 * sizeof(float), NULL, GL_DYNAMIC_DRAW);

	gfx.sprite_quad_buffer = malloc(GFX_MAX_SPRITE_QUADS * 4 * 4 * sizeof(float));

	if (!gfx.sprite_quad_buffer)
	{
		fprintf(stderr, "Failed to allocate memory for sprite quad buffer\n");
		return 0;
	}

	// Index buffer

	indices = malloc(GFX_MAX_SPRITE_QUADS * 6 * sizeof(unsigned int));
	if (!indices)
	{
		fprintf(stderr, "Failed to allocate memory for sprite quad indices");
		return 0;
	}

	for (int i = 0; i < GFX_MAX_SPRITE_QUADS; i++)
	{
		indices[i * 6 + 0] = i * 4 + 0;
		indices[i * 6 + 1] = i * 4 + 1;
		indices[i * 6 + 2] = i * 4 + 2;
		indices[i * 6 + 3] = i * 4 + 2;
		indices[i * 6 + 4] = i * 4 + 1;
		indices[i * 6 + 5] = i * 4 + 3;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gfx.sprite_quad_mesh.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, GFX_MAX_SPRITE_QUADS * 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	free(indices);

	// a_Pos
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	// glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

	// a_TexCoord
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void*)(2 * sizeof(float)));

	glBindVertexArray(0);

	// Setup viewport
	gfx.viewport = platform_get_viewport_size();
	gfx_shader_unform_mat4(&gfx.colour_shader, mat4_custom_projection(vec2_new(1280.0f, 720.0f)), "u_Projection");
	gfx_shader_unform_mat4(&gfx.texture_shader, mat4_custom_projection(vec2_new(1280.0f, 720.0f)), "u_Projection");

	// Print platform info
	gfx_print_platform_info();

	return 1;
}

void gfx_shutdown()
{
	gfx_shader_destroy(&gfx.colour_shader);
	gfx_shader_destroy(&gfx.texture_shader);
	gfx_mesh_destroy(&gfx.colour_quad_mesh);
	gfx_mesh_destroy(&gfx.sprite_quad_mesh);

	free(gfx.colour_quad_buffer);
	free(gfx.sprite_quad_buffer);
}

void gfx_print_platform_info()
{
	printf("Graphics platform info:\n");
	printf("Maximum texture units: %d\n", gfx.info.max_texture_units);
	printf("\n");
}

void gfx_mesh_destroy(Mesh *mesh)
{
	glDeleteVertexArrays(1, &mesh->vao);
	glDeleteBuffers(1, &mesh->vbo);
	glDeleteBuffers(1, &mesh->ibo);
}

void gfx_start_frame(vec4 bg_colour)
{
	if (platform_viewport_changed())
	{
		gfx.viewport = platform_get_viewport_size();
		glViewport(0, 0, gfx.viewport.x, gfx.viewport.y);
	}

	glClearColor(bg_colour.r, bg_colour.g, bg_colour.b, bg_colour.a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void gfx_end_frame()
{
	gfx_flush_colour_quads();
	gfx_flush_sprite_quads();
}

void gfx_draw_quad(Gfx_Quad quad)
{
	switch(quad.type)
	{
		case GFX_QUAD_COLOUR:
		{
			if (gfx.colour_quad_count >= GFX_MAX_COLOUR_QUADS)
			{
				printf("DEBUG: FLUSHING COLOUR QUADS");
				gfx_flush_colour_quads();
			}

			gfx.colour_quads[gfx.colour_quad_count] = quad;
			gfx.colour_quad_count++;
		}
		break;
		case GFX_QUAD_SPRITE:
		{
			if (gfx.sprite_quad_count >= GFX_MAX_SPRITE_QUADS)
			{
				printf("DEBUG: FLUSHING SPRITE QUADS");
				gfx_flush_sprite_quads();
			}

			gfx.sprite_quads[gfx.sprite_quad_count] = quad;
			gfx.sprite_quad_count++;
		}
		break;
	}
}

// TODO: Rotations don't really make any sense for bottom left origin
void gfx_flush_colour_quads()
{
	for (int i = 0; i < gfx.colour_quad_count; i++)
	{
		Quad_Vertices v;

		v = maths_get_quad_vertcies(gfx.colour_quads[i].quad);

		// Bottom left
		gfx.colour_quad_buffer[i * 20 + 0] = v.bottom_left.x;
		gfx.colour_quad_buffer[i * 20 + 1] = v.bottom_left.y;
		gfx.colour_quad_buffer[i * 20 + 2] = gfx.colour_quads[i].colour.r;
		gfx.colour_quad_buffer[i * 20 + 3] = gfx.colour_quads[i].colour.g;
		gfx.colour_quad_buffer[i * 20 + 4] = gfx.colour_quads[i].colour.b;

		// Bottom right
		gfx.colour_quad_buffer[i * 20 + 5] = v.bottom_right.x;
		gfx.colour_quad_buffer[i * 20 + 6] = v.bottom_right.y;
		gfx.colour_quad_buffer[i * 20 + 7] = gfx.colour_quads[i].colour.r;
		gfx.colour_quad_buffer[i * 20 + 8] = gfx.colour_quads[i].colour.g;
		gfx.colour_quad_buffer[i * 20 + 9] = gfx.colour_quads[i].colour.b;

		// Top left
		gfx.colour_quad_buffer[i * 20 + 10] = v.top_left.x;
		gfx.colour_quad_buffer[i * 20 + 11] = v.top_left.y;
		gfx.colour_quad_buffer[i * 20 + 12] = gfx.colour_quads[i].colour.r;
		gfx.colour_quad_buffer[i * 20 + 13] = gfx.colour_quads[i].colour.g;
		gfx.colour_quad_buffer[i * 20 + 14] = gfx.colour_quads[i].colour.b;

		// Top right
		gfx.colour_quad_buffer[i * 20 + 15] = v.top_right.x;
		gfx.colour_quad_buffer[i * 20 + 16] = v.top_right.y;
		gfx.colour_quad_buffer[i * 20 + 17] = gfx.colour_quads[i].colour.r;
		gfx.colour_quad_buffer[i * 20 + 18] = gfx.colour_quads[i].colour.g;
		gfx.colour_quad_buffer[i * 20 + 19] = gfx.colour_quads[i].colour.b;
	}

	glBindBuffer(GL_ARRAY_BUFFER, gfx.colour_quad_mesh.vbo);
	glBufferData(GL_ARRAY_BUFFER, gfx.colour_quad_count * 4 * 5 * sizeof(float), gfx.colour_quad_buffer, GL_DYNAMIC_DRAW);

	glBindVertexArray(gfx.colour_quad_mesh.vao);
	glUseProgram(gfx.colour_shader.id);

	glDrawElements(GL_TRIANGLES, gfx.colour_quad_count * 6, GL_UNSIGNED_INT, NULL);

	gfx.colour_quad_count = 0;
}

int	gfx_shader_create(Shader *out_shader, const char *vertex_path, const char *fragment_path)
{
	char *buffer;
	size_t length;
	GLuint prog, vert, frag;
	GLint success;
	char info_log[1024];

	if (!platform_read_file_alloc(vertex_path, &buffer, &length))
		return 0;

	vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert, 1, (const char * const *)&buffer, NULL);
	glCompileShader(vert);

	glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		fprintf(stderr, "Failed to compile vertex shader %s\n", vertex_path);
		glDeleteShader(vert);
		glGetShaderInfoLog(vert, 1024, NULL, info_log);
		fprintf(stderr, "%s\n", info_log);
		return 0;
	}

	free(buffer);

	if (!platform_read_file_alloc(fragment_path, &buffer, &length))
		return 0;

	frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, (const char * const *)&buffer, NULL);
	glCompileShader(frag);

	glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		fprintf(stderr, "Failed to compile fragment shader %s\n", fragment_path);
		glDeleteShader(vert);
		glDeleteShader(frag);
		glGetShaderInfoLog(frag, 1024, NULL, info_log);
		fprintf(stderr, "%s\n", info_log);
		return 0;
	}

	prog = glCreateProgram();
	glAttachShader(prog, vert);
	glAttachShader(prog, frag);
	glLinkProgram(prog);

	glGetProgramiv(prog, GL_LINK_STATUS, &success);
	if (!success)
	{
		fprintf(stderr, "Failed to link program (%s, %s)\n", vertex_path, fragment_path);
		glDeleteProgram(prog);
		glDeleteShader(vert);
		glDeleteShader(frag);
		glGetProgramInfoLog(prog, 1024, NULL, info_log);
		fprintf(stderr, "%s\n", info_log);
		return 0;
	}

	free(buffer);

	glDeleteShader(vert);
	glDeleteShader(frag);

	out_shader->id = prog;

	return 1;
}

void gfx_shader_destroy(Shader *shader)
{
	glDeleteProgram(shader->id);
}

void gfx_shader_unform_mat4(Shader *shader, mat4 mat, const char *uniform_name)
{
	GLint location;

	location = glGetUniformLocation(shader->id, uniform_name);
	if (location == -1)
	{
		fprintf(stderr, "Uniform %s cannot be found\n", uniform_name);
		return;
	}

	glUseProgram(shader->id);
	glUniformMatrix4fv(location, 1, GL_TRUE, (GLfloat*) mat.a);
}

Gfx_Quad gfx_quad_colour_centred(vec2 position, vec2 size, vec4 colour, float angle)
{
	Gfx_Quad q;
	q.quad.position = position;
	q.quad.size = size;
	q.quad.angle = angle;
	q.quad.origin = GFX_ORIGIN_CENTRE;
	q.type = GFX_QUAD_COLOUR;
	q.colour = colour;
	return q;
}

void gfx_flush_sprite_quads()
{
	for (int i = 0; i < gfx.sprite_quad_count; i++)
	{
		Gfx_Quad *quad;
		Quad_Vertices v;

		quad = &gfx.sprite_quads[i];
		v = maths_get_quad_vertcies(quad->quad);

		gfx.sprite_quad_buffer[i * 16 + 0] = v.bottom_left.x;
		gfx.sprite_quad_buffer[i * 16 + 1] = v.bottom_left.y;
		gfx.sprite_quad_buffer[i * 16 + 2] = quad->sprite.tex_coord_origin.x;
		gfx.sprite_quad_buffer[i * 16 + 3] = quad->sprite.tex_coord_origin.y;

		gfx.sprite_quad_buffer[i * 16 + 4] = v.bottom_right.x;
		gfx.sprite_quad_buffer[i * 16 + 5] = v.bottom_right.y;
		gfx.sprite_quad_buffer[i * 16 + 6] = quad->sprite.tex_coord_origin.x + quad->sprite.tex_coord_size.x;
		gfx.sprite_quad_buffer[i * 16 + 7] = quad->sprite.tex_coord_origin.y;

		gfx.sprite_quad_buffer[i * 16 + 8] = v.top_left.x;
		gfx.sprite_quad_buffer[i * 16 + 9] = v.top_left.y;
		gfx.sprite_quad_buffer[i * 16 + 10] = quad->sprite.tex_coord_origin.x;
		gfx.sprite_quad_buffer[i * 16 + 11] = quad->sprite.tex_coord_origin.y + quad->sprite.tex_coord_size.y;

		gfx.sprite_quad_buffer[i * 16 + 12] = v.top_right.x;
		gfx.sprite_quad_buffer[i * 16 + 13] = v.top_right.y;
		gfx.sprite_quad_buffer[i * 16 + 14] = quad->sprite.tex_coord_origin.x + quad->sprite.tex_coord_size.x;
		gfx.sprite_quad_buffer[i * 16 + 15] = quad->sprite.tex_coord_origin.y + quad->sprite.tex_coord_size.y;
	}

	// printf("DEBUG: FLUSHING SPRITE DATA\n");
	// printf("(%f, %f), (%f, %f)\n", gfx.sprite_quad_buffer[0], gfx.sprite_quad_buffer[1], gfx.sprite_quad_buffer[2], gfx.sprite_quad_buffer[3]);
	// printf("(%f, %f), (%f, %f)\n", gfx.sprite_quad_buffer[4], gfx.sprite_quad_buffer[5], gfx.sprite_quad_buffer[6], gfx.sprite_quad_buffer[7]);
	// printf("(%f, %f), (%f, %f)\n", gfx.sprite_quad_buffer[8], gfx.sprite_quad_buffer[9], gfx.sprite_quad_buffer[10], gfx.sprite_quad_buffer[11]);
	// printf("(%f, %f), (%f, %f)\n", gfx.sprite_quad_buffer[12], gfx.sprite_quad_buffer[13], gfx.sprite_quad_buffer[14], gfx.sprite_quad_buffer[15]);
	// printf("DRAWING %d QUAD(S)\n", gfx.sprite_quad_count);
	

	glBindBuffer(GL_ARRAY_BUFFER, gfx.sprite_quad_mesh.vbo);
	glBufferData(GL_ARRAY_BUFFER, gfx.sprite_quad_count * 4 * 4 * sizeof(float), gfx.sprite_quad_buffer, GL_DYNAMIC_DRAW);

	glBindVertexArray(gfx.sprite_quad_mesh.vao);
	glUseProgram(gfx.texture_shader.id);

	// gfx_shader_uniform_int(&gfx.texture_shader, 0, "u_Texture");

	glDrawElements(GL_TRIANGLES, gfx.sprite_quad_count * 6, GL_UNSIGNED_INT, NULL);

	gfx.sprite_quad_count = 0;
}

int	gfx_texture_create(Texture *out_texture, const char *texture_path)
{
	int 			width, height, channels;
	unsigned char 	*buffer;
	GLuint 			texture;

	stbi_set_flip_vertically_on_load(1);
	buffer = stbi_load(texture_path, &width, &height, &channels, 0);

	if (!buffer)
	{
		fprintf(stderr, "Failed to load texture %s\n", texture_path);
		return 0;
	}

	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, (const void*)buffer);

	out_texture->id = texture;

	return 1;
}

void gfx_shader_uniform_int(Shader *shader, int value, const char *uniform_name)
{
	GLint location;

	location = glGetUniformLocation(shader->id, uniform_name);
	if (location == -1)
	{
		fprintf(stderr, "Uniform %s cannot be found\n", uniform_name);
		return;
	}

	glUseProgram(shader->id);
	glUniform1i(location, value);
}

void gfx_opengl_error_callback(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam)
{
//   fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n\n",
        //    ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            // type, severity, message );
	
	switch (type)
	{
		case GL_DEBUG_TYPE_ERROR:
			fprintf(stderr, "** GL ERROR **: %s\n", message);
			break;
		case GL_DEBUG_TYPE_PERFORMANCE:
			printf("** GL PERFORMANCE **: %s\n", message);
			break;
		default:
			break;
	}
}