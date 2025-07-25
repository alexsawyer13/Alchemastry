#include "platform.h"

#include "core/log.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <math.h>

Platform platform;

int platform_init()
{
	if (glfwInit() == GLFW_FALSE)
	{
		log_fatal("Failed to initialise GLFW\n");
		return 0;
	}
	
	platform.window = glfwCreateWindow(1600, 900, "Alchemastry", NULL, NULL);
	// platform.window = glfwCreateWindow(1920, 1080, "Alchemastry", glfwGetPrimaryMonitor(), NULL);

	if (!platform.window)
	{
		log_fatal("Failed to create GLFW window\n");
		return 0;
	}

	glfwSetKeyCallback(platform.window, _platform_key_callback);
	glfwSetMouseButtonCallback(platform.window, _platform_mouse_button_callback);

	for (int i = 0; i < GLFW_KEY_LAST; i++)
	{
		platform.keys[i] = PLATFORM_KEY_UP;
	}

	for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
	{
		platform.mouse[i] = PLATFORM_KEY_UP;
	}

	glfwSetWindowAspectRatio(platform.window, 16, 9);
	platform.viewport = platform_get_viewport_size();

	glfwMakeContextCurrent(platform.window);
	glfwSwapInterval(1);

	platform.current_time = glfwGetTime();
	platform.frame_count = 0;

	srand(time(0));

	return 1;
}

void platform_shutdown()
{
	glfwDestroyWindow(platform.window);
	glfwTerminate();
}

void platform_window_close()
{
	glfwSetWindowShouldClose(platform.window, 1);
}

int platform_window_closed()
{
	return glfwWindowShouldClose(platform.window);
}

void platform_update()
{
	ivec2	new_viewport;
	float	current_time;
	char	window_title[64];

	for (int i = 0; i < GLFW_KEY_LAST; i++)
	{
		if (platform.keys[i] == PLATFORM_KEY_PRESSED)
			platform.keys[i] = PLATFORM_KEY_DOWN;
		if (platform.keys[i] == PLATFORM_KEY_RELEASED)
			platform.keys[i] = PLATFORM_KEY_UP;
	}

	for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
	{
		if (platform.mouse[i] == PLATFORM_KEY_PRESSED)
			platform.mouse[i] = PLATFORM_KEY_DOWN;
		if (platform.mouse[i] == PLATFORM_KEY_RELEASED)
			platform.mouse[i] = PLATFORM_KEY_UP;
	}

	glfwSwapBuffers(platform.window);
	glfwPollEvents();

	new_viewport = platform_get_viewport_size();

	platform.viewport_changed = !ivec2_eq(platform.viewport, new_viewport);
	platform.viewport = new_viewport;

	current_time = (float)glfwGetTime();
	platform.delta_time = current_time - platform.current_time;
	platform.current_time = current_time;

	if (platform.frame_count % 30 == 0)
	{
		sprintf(window_title, "Alchemastry - %d FPS", (int)roundf(1.0f / platform.delta_time));
		glfwSetWindowTitle(platform.window, window_title);
	}

	platform.frame_count++;
}

GLADloadproc platform_get_gladloadproc()
{
	return (GLADloadproc) glfwGetProcAddress;
}

int platform_read_file_alloc(const char *path, char **out_buffer, size_t *out_length)
{
	int error;
	FILE *file;
	size_t length, output;
	char *buffer;

	file = fopen(path, "r");
	if (!file)
	{
		log_error("Failed to open file %s with error code %d: %s\n", path, errno, strerror(errno));
		return 0;
	}

	fseek(file, 0, SEEK_END);
	length = (size_t)ftell(file);
	fseek(file, 0, SEEK_SET);

	if (length == 0)
	{
		log_error("Trying to read file %s but it's empty\n", path);
		fclose(file);
		return 0;
	}

	buffer = malloc(length + 1);
	if (!buffer)
	{
		log_error("Failed to allocate buffer for file %s\n", path);
		fclose(file);
		return 0;
	}

	output = fread(buffer, 1, length, file);
	if (output == 0)
	{
		if (ferror(file))
		{
			log_error("Failed to read file %s\n", path);
			fclose(file);
			return 0;
		}
	}
	if (output > length)
	{
		log_error("Read more characters from file %s than allocated\n", path);
		fclose(file);
		return 0;
	}

	fclose(file);

	buffer[output] = '\0';

	*out_buffer = buffer;

	if (out_length)
		*out_length = output;

	return 1;
}

ivec2 platform_get_viewport_size()
{
	ivec2 viewport;
	glfwGetWindowSize(platform.window, &viewport.x, &viewport.y);
	return viewport;
}

int platform_viewport_changed()
{
	return platform.viewport_changed;
}

int	platform_key_down(int keycode)
{
	return (platform.keys[keycode] == PLATFORM_KEY_DOWN) || (platform.keys[keycode] == PLATFORM_KEY_PRESSED);
}

int	platform_key_pressed(int keycode)
{
	return platform.keys[keycode] == PLATFORM_KEY_PRESSED;
}

int	platform_key_released(int keycode)
{
	return platform.keys[keycode] == PLATFORM_KEY_RELEASED;
}

float	platform_time()
{
	return platform.current_time;
}

float platform_delta_time()
{
	return platform.delta_time;
}

float platform_random()
{
	return (float)rand() / RAND_MAX;
}

vec2 platform_mouse_position()
{
	vec2 pos;
	double x;
	double y;

	glfwGetCursorPos(platform.window, &x, &y);

	pos.x = (float)x;
	pos.y = (float)y;

	return pos;
}

void _platform_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch(action)
	{
		case GLFW_PRESS:
			platform.keys[key] = PLATFORM_KEY_PRESSED;
			break;
		
		case GLFW_RELEASE:
			platform.keys[key] = PLATFORM_KEY_RELEASED;
			break;

		default:
			break;
	}
}

void _platform_mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
	switch(action)
	{
		case GLFW_PRESS:
			platform.mouse[button] = PLATFORM_KEY_PRESSED;
			break;
		
		case GLFW_RELEASE:
			platform.mouse[button] = PLATFORM_KEY_RELEASED;
			break;

		default:
			break;
	}
}

int platform_mouse_down(int button)
{
	return (platform.mouse[button] == PLATFORM_KEY_DOWN) || (platform.mouse[button] == PLATFORM_KEY_PRESSED);
}

int	platform_mouse_pressed(int button)
{
	return platform.mouse[button] == PLATFORM_KEY_PRESSED;
}

int	platform_mouse_released(int button)
{
	return platform.mouse[button] == PLATFORM_KEY_RELEASED;
}
