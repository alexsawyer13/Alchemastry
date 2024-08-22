#include "gfx.h"
#include "platform.h"

#include <glad/glad.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

Gfx gfx;

int gfx_init()
{
	// Load OpenGL function pointers
	gladLoadGLLoader(platform_get_gladloadproc());
	
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

	unsigned int *indices = malloc(GFX_MAX_COLOUR_QUADS * 6 * sizeof(unsigned int));
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
	glGenBuffers(1, &gfx.sprite_quad_mesh.vbo);
	glGenBuffers(1, &gfx.sprite_quad_mesh.ibo);

	glBindVertexArray(gfx.sprite_quad_mesh.vao);

	// Vertex buffer

	glBindBuffer(GL_ARRAY_BUFFER, gfx.sprite_quad_mesh.vbo);
	glBufferData(GL_ARRAY_BUFFER, GFX_MAX_SPRITE_QUADS * 4 * 4 * sizeof(float), NULL, GL_DYNAMIC_DRAW);

	gfx.sprite_quad_buffer = malloc(GFX_MAX_SPRITE_QUADS * 4 * 5 * sizeof(float));

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

	// a_TexCoord
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void *)(2 * sizeof(float)));

	glBindVertexArray(0);

	// Setup viewport
	gfx.viewport = platform_get_viewport_size();
	gfx_shader_unform_mat4(&gfx.colour_shader, mat4_custom_projection(vec2_new(1280.0f, 720.0f)), "u_Projection");

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

void gfx_draw_quad(Quad quad)
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
		Quad *quad;
		vec2 centre;
		vec2 half_size;
		mat2 rotation;
		vec2 bl_offset;
		vec2 br_offset;
		vec2 tl_offset;
		vec2 tr_offset;

		quad = &gfx.colour_quads[i];
		half_size = vec2_mul_float(quad->size, 0.5f);

		switch (quad->origin)
		{
		case GFX_ORIGIN_CENTRE:
			centre = quad->position;
			break;

		case GFX_ORIGIN_BOTTOM_LEFT:
			fprintf(stderr, "WARNING, ROTATIONS DON'T WORK AS INTENDED FOR BOTTOM LEFT ORIGIN\n");
			centre = vec2_add_vec2(quad->position, half_size);
			break;

		default:
			fprintf(stderr, "CANNOT DRAW QUAD FROM THE SPECIFIED CENTRE %d\n", quad->origin);
			exit(EXIT_FAILURE);
			break;
		}

		rotation = mat2_rotation_cw(quad->angle);
		bl_offset = mat2_mul_vec2(rotation, vec2_new(-half_size.x, -half_size.y));
		br_offset = mat2_mul_vec2(rotation, vec2_new(+half_size.x, -half_size.y));
		tl_offset = mat2_mul_vec2(rotation, vec2_new(-half_size.x, +half_size.y));
		tr_offset = mat2_mul_vec2(rotation, vec2_new(+half_size.x, +half_size.y));

		// Bottom left
		gfx.colour_quad_buffer[i * 20 + 0] = centre.x + bl_offset.x;
		gfx.colour_quad_buffer[i * 20 + 1] = centre.y + bl_offset.y;
		gfx.colour_quad_buffer[i * 20 + 2] = gfx.colour_quads[i].colour.r;
		gfx.colour_quad_buffer[i * 20 + 3] = gfx.colour_quads[i].colour.g;
		gfx.colour_quad_buffer[i * 20 + 4] = gfx.colour_quads[i].colour.b;

		// Bottom right
		gfx.colour_quad_buffer[i * 20 + 5] = centre.x + br_offset.x;
		gfx.colour_quad_buffer[i * 20 + 6] = centre.y + br_offset.y;
		gfx.colour_quad_buffer[i * 20 + 7] = gfx.colour_quads[i].colour.r;
		gfx.colour_quad_buffer[i * 20 + 8] = gfx.colour_quads[i].colour.g;
		gfx.colour_quad_buffer[i * 20 + 9] = gfx.colour_quads[i].colour.b;

		// Top left
		gfx.colour_quad_buffer[i * 20 + 10] = centre.x + tl_offset.x;
		gfx.colour_quad_buffer[i * 20 + 11] = centre.y + tl_offset.y;
		gfx.colour_quad_buffer[i * 20 + 12] = gfx.colour_quads[i].colour.r;
		gfx.colour_quad_buffer[i * 20 + 13] = gfx.colour_quads[i].colour.g;
		gfx.colour_quad_buffer[i * 20 + 14] = gfx.colour_quads[i].colour.b;

		// Top right
		gfx.colour_quad_buffer[i * 20 + 15] = centre.x + tr_offset.x;
		gfx.colour_quad_buffer[i * 20 + 16] = centre.y + tr_offset.y;
		gfx.colour_quad_buffer[i * 20 + 17] = gfx.colour_quads[i].colour.r;
		gfx.colour_quad_buffer[i * 20 + 18] = gfx.colour_quads[i].colour.g;
		gfx.colour_quad_buffer[i * 20 + 19] = gfx.colour_quads[i].colour.b;

		//// Bottom left
		//gfx.colour_quad_buffer[i * 20 + 0]  = gfx.colour_quads[i].position.x;
		//gfx.colour_quad_buffer[i * 20 + 1]  = gfx.colour_quads[i].position.y;
		//gfx.colour_quad_buffer[i * 20 + 2]  = gfx.colour_quads[i].colour.r;
		//gfx.colour_quad_buffer[i * 20 + 3]  = gfx.colour_quads[i].colour.g;
		//gfx.colour_quad_buffer[i * 20 + 4]  = gfx.colour_quads[i].colour.b;

		//// Bottom right
		//gfx.colour_quad_buffer[i * 20 + 5]  = gfx.colour_quads[i].position.x + gfx.colour_quads[i].size.x;
		//gfx.colour_quad_buffer[i * 20 + 6]  = gfx.colour_quads[i].position.y;
		//gfx.colour_quad_buffer[i * 20 + 7]  = gfx.colour_quads[i].colour.r;
		//gfx.colour_quad_buffer[i * 20 + 8]  = gfx.colour_quads[i].colour.g;
		//gfx.colour_quad_buffer[i * 20 + 9]  = gfx.colour_quads[i].colour.b;

		//// Top left
		//gfx.colour_quad_buffer[i * 20 + 10] = gfx.colour_quads[i].position.x;
		//gfx.colour_quad_buffer[i * 20 + 11] = gfx.colour_quads[i].position.y + gfx.colour_quads[i].size.y;
		//gfx.colour_quad_buffer[i * 20 + 12] = gfx.colour_quads[i].colour.r;
		//gfx.colour_quad_buffer[i * 20 + 13] = gfx.colour_quads[i].colour.g;
		//gfx.colour_quad_buffer[i * 20 + 14] = gfx.colour_quads[i].colour.b;

		//// Top right
		//gfx.colour_quad_buffer[i * 20 + 15] = gfx.colour_quads[i].position.x + gfx.colour_quads[i].size.x;
		//gfx.colour_quad_buffer[i * 20 + 16] = gfx.colour_quads[i].position.y + gfx.colour_quads[i].size.y;
		//gfx.colour_quad_buffer[i * 20 + 17] = gfx.colour_quads[i].colour.r;
		//gfx.colour_quad_buffer[i * 20 + 18] = gfx.colour_quads[i].colour.g;
		//gfx.colour_quad_buffer[i * 20 + 19] = gfx.colour_quads[i].colour.b;
	}

	glBindVertexArray(gfx.colour_quad_mesh.vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gfx.colour_quad_mesh.ibo);
	glBufferData(GL_ARRAY_BUFFER, gfx.colour_quad_count * 4 * 5 * sizeof(float), gfx.colour_quad_buffer, GL_DYNAMIC_DRAW);
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
	}

	glUseProgram(shader->id);
	glUniformMatrix4fv(location, 1, GL_TRUE, (GLfloat*) mat.a);
}

Quad gfx_quad_colour_centred(vec2 position, vec2 size, vec4 colour, float angle)
{
	Quad q;
	q.position = position;
	q.size = size;
	q.angle = angle;
	q.origin = GFX_ORIGIN_CENTRE;
	q.type = GFX_QUAD_COLOUR;
	q.colour = colour;
	return q;
}

void gfx_flush_sprite_quads()
{
	if (gfx.sprite_quad_count == 0)
		return;

	fprintf(stderr, "FLUSHING SPRITE QUADS IS NOT YET IMPLEMENTED\n");
	exit(EXIT_FAILURE);
}