#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include "maths.h"

#include <GLFW/glfw3.h>

typedef struct
{
	GLFWwindow	*window;

	ivec2		viewport;
	int			viewport_changed;

	double		current_time;
	float		delta_time;
} Platform;

int 	platform_init();
void 	platform_shutdown();

void 	platform_update();

int 	platform_window_closed();
void	platform_window_close();

ivec2 	platform_get_viewport_size();
int 	platform_viewport_changed();

int		platform_key_down(int keycode);

float	platform_delta_time();

float	platform_random();

int 	platform_read_file_alloc(const char *path, char **out_buffer, size_t *out_length);

typedef void *(*GLADloadproc)(const char *name);
GLADloadproc platform_get_gladloadproc();

#endif