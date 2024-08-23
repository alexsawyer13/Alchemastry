#include "platform.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

Platform platform;

int platform_init()
{
	if (glfwInit() == GLFW_FALSE)
	{
		fprintf(stderr, "Failed to initialise GLFW\n");
		return 0;
	}
	
	// platform.window = glfwCreateWindow(1600, 900, "Alchemastry", NULL, NULL);
	platform.window = glfwCreateWindow(1920, 1080, "Alchemastry", glfwGetPrimaryMonitor(), NULL);
	glfwSetWindowAspectRatio(platform.window, 16, 9);
	platform.viewport = platform_get_viewport_size();

	if (!platform.window)
	{
		fprintf(stderr, "Failed to create GLFW window\n");
		return 0;
	}

	glfwMakeContextCurrent(platform.window);

	return 1;
}

void platform_shutdown()
{
	glfwDestroyWindow(platform.window);
	glfwTerminate();
}

int platform_window_closed()
{
	return glfwWindowShouldClose(platform.window);
}

void platform_update()
{
	ivec2 new_viewport;

	glfwSwapBuffers(platform.window);
	glfwPollEvents();

	// TEMP
	if (glfwGetKey(platform.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(platform.window, 1);

	new_viewport = platform_get_viewport_size();

	platform.viewport_changed = !ivec2_eq(platform.viewport, new_viewport);
	platform.viewport = new_viewport;
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
		fprintf(stderr, "Failed to open file %s with error code %d: %s\n", path, errno, strerror(errno));
		return 0;
	}

	fseek(file, 0, SEEK_END);
	length = (size_t)ftell(file);
	fseek(file, 0, SEEK_SET);

	if (length == 0)
	{
		fprintf(stderr, "Trying to read file %s but it's empty\n", path);
		fclose(file);
		return 0;
	}

	buffer = malloc(length + 1);
	if (!buffer)
	{
		fprintf(stderr, "Failed to allocate buffer for file %s\n", path);
		fclose(file);
		return 0;
	}

	output = fread(buffer, 1, length, file);
	if (output == 0)
	{
		if (ferror(file))
		{
			fprintf(stderr, "Failed to read file %s\n", path);
			fclose(file);
			return 0;
		}
	}
	if (output > length)
	{
		fprintf(stderr, "Read more characters from file %s than allocated\n", path);
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