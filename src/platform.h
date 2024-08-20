#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include <GLFW/glfw3.h> // TODO: GET RID OF THIS

typedef struct
{
	GLFWwindow *window;
} Platform;

int platform_init();
void platform_shutdown();

int platform_window_closed();
void platform_update();

typedef void *(*GLADloadproc)(const char *name);
GLADloadproc platform_get_gladloadproc();

#endif