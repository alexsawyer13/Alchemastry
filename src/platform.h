#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include "maths.h"

#include <GLFW/glfw3.h>

typedef struct
{
	GLFWwindow *window;

	ivec2 viewport;
	int viewport_changed;
} Platform;

int 	platform_init();
void 	platform_shutdown();

int 	platform_window_closed();
void 	platform_update();

ivec2 	platform_get_viewport_size();
int 	platform_viewport_changed();

int 	platform_read_file_alloc(const char *path, char **out_buffer, size_t *out_length);

typedef void *(*GLADloadproc)(const char *name);
GLADloadproc platform_get_gladloadproc();

#endif