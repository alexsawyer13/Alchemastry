#include "gfx.h"
#include "platform.h"

#include <glad/glad.h>
#include <stb/stb_image.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

Gfx gfx;

const char *texture_shader_vertex_source =
"#version 450 core\n"
"layout (location = 0) in vec2 a_Pos;\n"
"layout (location = 1) in vec2 a_TexCoord;\n"
"layout (location = 2) in float  a_TextureUnit;\n"
"out vec2 v_TexCoord;\n"
"flat out float v_TextureUnit;\n"
"uniform mat4 u_Projection;\n"
"void main()\n"
"{\n"
"	gl_Position = u_Projection * vec4(a_Pos, 0.0, 1.0);\n"
"	v_TexCoord = a_TexCoord;\n"
"	v_TextureUnit = a_TextureUnit;"
"}\n";

const char *texture_shader_fragment_source =
"#version 450 core\n"
"in vec2 v_TexCoord;\n"
"flat in float v_TextureUnit;\n"
"out vec4 o_Colour;\n"
"uniform sampler2D u_Texture[16];\n"
"void main()\n"
"{\n"
"int index = int(v_TextureUnit);"
"o_Colour = texture(u_Texture[index], v_TexCoord);\n"
"}\n"
"\n";

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

	// Load colour shader
	if (!gfx_shader_create_from_file(&gfx.colour_shader, "assets/shaders/colour.vert", "assets/shaders/colour.frag"))
	{
		fprintf(stderr, "Failed to create colour shader\n");
		return 0;
	}

	// Generate and load texture shader
	printf("WARNING ONLY USING 16 TEXTURE UNITS WHEN MORE ARE AVAILABLE\n");
	if (!gfx_shader_create_from_source(&gfx.texture_shader, texture_shader_vertex_source, texture_shader_fragment_source))
	{
		fprintf(stderr, "Failed to create texture shader\n");
		return 0;
	}

	// TMP
	int ijraoij[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	gfx_shader_uniform_int_array(&gfx.texture_shader, 16, ijraoij, "u_Texture");

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
	glBufferData(GL_ARRAY_BUFFER, GFX_MAX_SPRITE_QUADS * 4 * 5 * sizeof(float), NULL, GL_DYNAMIC_DRAW);

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
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);

	// a_TexCoord
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)(2 * sizeof(float)));

	// a_TextureUnit
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)(4 * sizeof(float)));

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

int gfx_shader_create_from_source(Shader *out_shader, const char *vertex_source, const char *fragment_source)
{
	GLuint prog, vert, frag;
	GLint success;
	char info_log[1024];

	vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert, 1, (const char * const *)&vertex_source, NULL);
	glCompileShader(vert);

	glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		fprintf(stderr, "Failed to compile vertex shader \n%s\n", vertex_source);
		glDeleteShader(vert);
		glGetShaderInfoLog(vert, 1024, NULL, info_log);
		fprintf(stderr, "%s\n", info_log);
		return 0;
	}

	frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, (const char * const *)&fragment_source, NULL);
	glCompileShader(frag);

	glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		fprintf(stderr, "Failed to compile fragment shader \n%s\n", fragment_source);
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
		fprintf(stderr, "Failed to link program \n%s\n%s\n", vertex_source, fragment_source);
		glDeleteProgram(prog);
		glDeleteShader(vert);
		glDeleteShader(frag);
		glGetProgramInfoLog(prog, 1024, NULL, info_log);
		fprintf(stderr, "%s\n", info_log);
		return 0;
	}

	glDeleteShader(vert);
	glDeleteShader(frag);

	out_shader->id = prog;

	return 1;
}

int gfx_shader_create_from_file(Shader *out_shader, const char *vertex_path, const char *fragment_path)
{
	char 	*vertex_source;
	char 	*fragment_source;

	if (!platform_read_file_alloc(vertex_path, &vertex_source, NULL))
		return 0;
	
	if (!platform_read_file_alloc(fragment_path, &fragment_source, NULL))
		return 0;

	if (!gfx_shader_create_from_source(out_shader, vertex_source, fragment_source))
		return 0;

	free(vertex_source);
	free(fragment_source);

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
	q.quad.origin = MATHS_ORIGIN_CENTRE;
	q.type = GFX_QUAD_COLOUR;
	q.colour = colour;
	return q;
}

void gfx_flush_sprite_quads()
{
	int texture_slots[16];
	int filled_texture_slots;

	memset(texture_slots, 0, 64);
	filled_texture_slots = 0;

	for (int i = 0; i < gfx.sprite_quad_count; i++)
	{
		Gfx_Quad *quad;
		Quad_Vertices v;
		int current_texture_slot;

		quad = &gfx.sprite_quads[i];
		v = maths_get_quad_vertcies(quad->quad);

		// Check if quad's texture is bound
		current_texture_slot = -1;
		for (int i = 0; i < 16; i++)
		{
			if (texture_slots[i] == quad->sprite.texture.id)
			{
				current_texture_slot = i;
			}
		}

		// If it isn't, and there's space, bind it
		if (current_texture_slot == -1 && filled_texture_slots < 16)
		{
			texture_slots[filled_texture_slots] = quad->sprite.texture.id;
			current_texture_slot = filled_texture_slots;
			filled_texture_slots++;
		}

		// If it is now bound, batch it

		if (current_texture_slot >= 0)
		{
			gfx.sprite_quad_buffer[i * 20 + 0] = v.bottom_left.x;
			gfx.sprite_quad_buffer[i * 20 + 1] = v.bottom_left.y;
			gfx.sprite_quad_buffer[i * 20 + 2] = quad->sprite.tex_coord_origin.x;
			gfx.sprite_quad_buffer[i * 20 + 3] = quad->sprite.tex_coord_origin.y;
			gfx.sprite_quad_buffer[i * 20 + 4] = current_texture_slot;

			gfx.sprite_quad_buffer[i * 20 + 5] = v.bottom_right.x;
			gfx.sprite_quad_buffer[i * 20 + 6] = v.bottom_right.y;
			gfx.sprite_quad_buffer[i * 20 + 7] = quad->sprite.tex_coord_origin.x + quad->sprite.tex_coord_size.x;
			gfx.sprite_quad_buffer[i * 20 + 8] = quad->sprite.tex_coord_origin.y;
			gfx.sprite_quad_buffer[i * 20 + 9] = current_texture_slot;

			gfx.sprite_quad_buffer[i * 20 + 10] = v.top_left.x;
			gfx.sprite_quad_buffer[i * 20 + 11] = v.top_left.y;
			gfx.sprite_quad_buffer[i * 20 + 12] = quad->sprite.tex_coord_origin.x;
			gfx.sprite_quad_buffer[i * 20 + 13] = quad->sprite.tex_coord_origin.y + quad->sprite.tex_coord_size.y;
			gfx.sprite_quad_buffer[i * 20 + 14] = current_texture_slot;

			gfx.sprite_quad_buffer[i * 20 + 15] = v.top_right.x;
			gfx.sprite_quad_buffer[i * 20 + 16] = v.top_right.y;
			gfx.sprite_quad_buffer[i * 20 + 17] = quad->sprite.tex_coord_origin.x + quad->sprite.tex_coord_size.x;
			gfx.sprite_quad_buffer[i * 20 + 18] = quad->sprite.tex_coord_origin.y + quad->sprite.tex_coord_size.y;
			gfx.sprite_quad_buffer[i * 20 + 19] = current_texture_slot;
		}

		// Otherwise put it away
		else
		{
			fprintf(stderr, "CANNOT YET RENDER MORE THAN 16 TEXTURES AT ONCE\n");
		}

	}

	// Bind appropriate textures
	for (int i = 0; i < filled_texture_slots; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, texture_slots[i]);
	}

	// Upload data to GPU
	glBindBuffer(GL_ARRAY_BUFFER, gfx.sprite_quad_mesh.vbo);
	glBufferData(GL_ARRAY_BUFFER, gfx.sprite_quad_count * 4 * 5 * sizeof(float), gfx.sprite_quad_buffer, GL_DYNAMIC_DRAW);
	
	// Render!!
	glBindVertexArray(gfx.sprite_quad_mesh.vao);
	glUseProgram(gfx.texture_shader.id);
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

void gfx_texture_bind(Texture *texture, int texture_unit)
{
	glActiveTexture(GL_TEXTURE0 + texture_unit);
	glBindTexture(GL_TEXTURE_2D, texture->id);
}

void gfx_shader_uniform_int_array(Shader *shader, int count, int *values, const char *uniform_name)
{
	GLint location;

	location = glGetUniformLocation(shader->id, uniform_name);
	if (location == -1)
	{
		fprintf(stderr, "Uniform %s cannot be found\n", uniform_name);
		return;
	}

	glUseProgram(shader->id);
	glUniform1iv(location, count, values);
}