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
		return 0;

	// Quad rendering
	gfx.quad_count = 0;

	glCreateVertexArrays(1, &gfx.quad_mesh.vao);
	glGenBuffers(1, &gfx.quad_mesh.vbo);
	glGenBuffers(1, &gfx.quad_mesh.ibo);

	glBindVertexArray(gfx.quad_mesh.vao);

	// Vertex buffer

	glBindBuffer(GL_ARRAY_BUFFER, gfx.quad_mesh.vbo);
	glBufferData(GL_ARRAY_BUFFER, GFX_MAX_QUADS * 4 * 5 * sizeof(float), NULL, GL_DYNAMIC_DRAW);

	gfx.quad_buffer = malloc(GFX_MAX_QUADS * 4 * 5 * sizeof(float));

	if (!gfx.quad_buffer)
	{
		fprintf(stderr, "Failed to allocate memory for quad buffer\n");
		return 0;
	}

	// Index buffer

	unsigned int *indices = malloc(GFX_MAX_QUADS * 6 * sizeof(unsigned int));
	if (!indices)
	{
		fprintf(stderr, "Failed to allocate memory for quad indices");
		return 0;
	}

	for (int i = 0; i < GFX_MAX_QUADS; i++)
	{
		indices[i * 6 + 0] = i * 4 + 0;
		indices[i * 6 + 1] = i * 4 + 1;
		indices[i * 6 + 2] = i * 4 + 2;
		indices[i * 6 + 3] = i * 4 + 2;
		indices[i * 6 + 4] = i * 4 + 1;
		indices[i * 6 + 5] = i * 4 + 3;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gfx.quad_mesh.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, GFX_MAX_QUADS * 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	free(indices);

	// a_Pos
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);

	// a_Colour
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 2 * sizeof(float));

	glBindVertexArray(0);

	// Setup viewport
	gfx.viewport = platform_get_viewport_size();
	gfx_shader_unform_mat4(&gfx.colour_shader, mat4_custom_projection(vec2_new(1280.0f, 720.0f)), "u_Projection");

	return 1;
}

void gfx_shutdown()
{
	gfx_shader_destroy(&gfx.colour_shader);
	gfx_mesh_destroy(&gfx.quad_mesh);

	free(gfx.quad_buffer);
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
	gfx_flush_quads();
}

void gfx_draw_quad(ColouredQuad *quad)
{
	if (gfx.quad_count >= GFX_MAX_QUADS)
	{
		printf("DEBUG: FLUSHING QUADS");
		gfx_flush_quads();
	}

	gfx.quads[gfx.quad_count] = *quad;
	gfx.quad_count++;
}

// TODO: Rotations don't really make any sense for bottom left origin
void gfx_flush_quads()
{
	for (int i = 0; i < gfx.quad_count; i++)
	{
		ColouredQuad *quad;
		vec2 centre;
		vec2 half_size;
		mat2 rotation;
		vec2 bl_offset;
		vec2 br_offset;
		vec2 tl_offset;
		vec2 tr_offset;

		quad = &gfx.quads[i];
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
		gfx.quad_buffer[i * 20 + 0] = centre.x + bl_offset.x;
		gfx.quad_buffer[i * 20 + 1] = centre.y + bl_offset.y;
		gfx.quad_buffer[i * 20 + 2] = gfx.quads[i].colour.r;
		gfx.quad_buffer[i * 20 + 3] = gfx.quads[i].colour.g;
		gfx.quad_buffer[i * 20 + 4] = gfx.quads[i].colour.b;

		// Bottom right
		gfx.quad_buffer[i * 20 + 5] = centre.x + br_offset.x;
		gfx.quad_buffer[i * 20 + 6] = centre.y + br_offset.y;
		gfx.quad_buffer[i * 20 + 7] = gfx.quads[i].colour.r;
		gfx.quad_buffer[i * 20 + 8] = gfx.quads[i].colour.g;
		gfx.quad_buffer[i * 20 + 9] = gfx.quads[i].colour.b;

		// Top left
		gfx.quad_buffer[i * 20 + 10] = centre.x + tl_offset.x;
		gfx.quad_buffer[i * 20 + 11] = centre.y + tl_offset.y;
		gfx.quad_buffer[i * 20 + 12] = gfx.quads[i].colour.r;
		gfx.quad_buffer[i * 20 + 13] = gfx.quads[i].colour.g;
		gfx.quad_buffer[i * 20 + 14] = gfx.quads[i].colour.b;

		// Top right
		gfx.quad_buffer[i * 20 + 15] = centre.x + tr_offset.x;
		gfx.quad_buffer[i * 20 + 16] = centre.y + tr_offset.y;
		gfx.quad_buffer[i * 20 + 17] = gfx.quads[i].colour.r;
		gfx.quad_buffer[i * 20 + 18] = gfx.quads[i].colour.g;
		gfx.quad_buffer[i * 20 + 19] = gfx.quads[i].colour.b;

		//// Bottom left
		//gfx.quad_buffer[i * 20 + 0]  = gfx.quads[i].position.x;
		//gfx.quad_buffer[i * 20 + 1]  = gfx.quads[i].position.y;
		//gfx.quad_buffer[i * 20 + 2]  = gfx.quads[i].colour.r;
		//gfx.quad_buffer[i * 20 + 3]  = gfx.quads[i].colour.g;
		//gfx.quad_buffer[i * 20 + 4]  = gfx.quads[i].colour.b;

		//// Bottom right
		//gfx.quad_buffer[i * 20 + 5]  = gfx.quads[i].position.x + gfx.quads[i].size.x;
		//gfx.quad_buffer[i * 20 + 6]  = gfx.quads[i].position.y;
		//gfx.quad_buffer[i * 20 + 7]  = gfx.quads[i].colour.r;
		//gfx.quad_buffer[i * 20 + 8]  = gfx.quads[i].colour.g;
		//gfx.quad_buffer[i * 20 + 9]  = gfx.quads[i].colour.b;

		//// Top left
		//gfx.quad_buffer[i * 20 + 10] = gfx.quads[i].position.x;
		//gfx.quad_buffer[i * 20 + 11] = gfx.quads[i].position.y + gfx.quads[i].size.y;
		//gfx.quad_buffer[i * 20 + 12] = gfx.quads[i].colour.r;
		//gfx.quad_buffer[i * 20 + 13] = gfx.quads[i].colour.g;
		//gfx.quad_buffer[i * 20 + 14] = gfx.quads[i].colour.b;

		//// Top right
		//gfx.quad_buffer[i * 20 + 15] = gfx.quads[i].position.x + gfx.quads[i].size.x;
		//gfx.quad_buffer[i * 20 + 16] = gfx.quads[i].position.y + gfx.quads[i].size.y;
		//gfx.quad_buffer[i * 20 + 17] = gfx.quads[i].colour.r;
		//gfx.quad_buffer[i * 20 + 18] = gfx.quads[i].colour.g;
		//gfx.quad_buffer[i * 20 + 19] = gfx.quads[i].colour.b;
	}

	glBindVertexArray(gfx.quad_mesh.vao);
	glBufferData(GL_ARRAY_BUFFER, gfx.quad_count * 4 * 5 * sizeof(float), gfx.quad_buffer, GL_DYNAMIC_DRAW);
	glUseProgram(gfx.colour_shader.id);
	glDrawElements(GL_TRIANGLES, gfx.quad_count * 6, GL_UNSIGNED_INT, NULL);

	gfx.quad_count = 0;
}

void gfx_draw_sprite(Sprite *sprite)
{
	if (gfx.sprite_count >= GFX_MAX_SPRITES)
	{
		printf("DEBUG: FLUSHING SPRITES");
		gfx_flush_sprites();
	}

	gfx.sprites[gfx.sprite_count] = *sprite;
	gfx.sprite_count++;

	fprintf(stderr, "SPRITE DRAWING NOT IMPLEMENTED YET");
	exit(EXIT_FAILURE);
}

void gfx_flush_sprites()
{
	fprintf(stderr, "SPRITE DRAWING NOT IMPLEMENTED YET");
	exit(EXIT_FAILURE);
}

int	gfx_shader_create(Shader *shader, const char *vertex_path, const char *fragment_path)
{
	char *buffer;
	size_t length;
	GLuint prog, vert, frag;
	GLint success;
	char info_log[1024];

	if (!platform_read_file_alloc(vertex_path, &buffer, &length))
		return 0;

	vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert, 1, &buffer, NULL);
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
	glShaderSource(frag, 1, &buffer, NULL);
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

	shader->id = prog;

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