#include "platform.h"

#include <stdio.h>

Platform platform;

int platform_init()
{
	if (glfwInit() == GLFW_FALSE)
	{
		fprintf(stderr, "Failed to initialise GLFW\n");
		return 0;
	}

	platform.window = glfwCreateWindow(1280, 720, "Hello world", NULL, NULL);

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
	glfwSwapBuffers(platform.window);
	glfwPollEvents();
}

GLADloadproc platform_get_gladloadproc()
{
	return glfwGetProcAddress;
}