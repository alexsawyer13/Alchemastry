#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include "maths.h"

#include <GLFW/glfw3.h>

typedef enum
{
	PLATFORM_KEY_PRESSED,
	PLATFORM_KEY_DOWN,
	PLATFORM_KEY_RELEASED,
	PLATFORM_KEY_UP
} Platform_Keystate;

typedef struct
{
	GLFWwindow		*window;

	ivec2			viewport;
	int			viewport_changed;

	float			current_time;
	float			delta_time;
	int			frame_count;

	Platform_Keystate 	keys[GLFW_KEY_LAST];
	Platform_Keystate	mouse[GLFW_MOUSE_BUTTON_LAST];
} Platform;

int 	platform_init();
void 	platform_shutdown();

void 	platform_update();

int 	platform_window_closed();
void	platform_window_close();

void 	_platform_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void	_platform_mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

int	platform_key_down(int keycode);
int	platform_key_pressed(int keycode);
int	platform_key_released(int keycode);

int	platform_mouse_down(int button);
int	platform_mouse_pressed(int button);
int	platform_mouse_released(int button);
vec2	platform_mouse_position();

ivec2 	platform_get_viewport_size();
int 	platform_viewport_changed();

float	platform_time();
float	platform_delta_time();

float	platform_random();

int 	platform_read_file_alloc(const char *path, char **out_buffer, size_t *out_length);

typedef void *(*GLADloadproc)(const char *name);
GLADloadproc platform_get_gladloadproc();

#endif
