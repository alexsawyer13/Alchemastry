c_files := src/main.c\
	   src/glad.c\
	   src/maths.c\
	   src/platform.c\
	   src/gfx.c\
	   src/game.c\
	   src/registry.c\

h_files := src/maths.h\
	   src/platform.h\
	   src/gfx.h\
	   src/game.h\

include_dirs := include/all\

lib_dirs := lib\gcc

libs := glfw3

.PHONY: Alchemaster

Alchemaster:
	gcc src/main.c vendor/glad.c src/maths.c src/platform.c src/gfx.c src/game.c src/registry.c -Iinclude/all -Llib/gcc -lglfw3 -lGL -lm -D GLFW_INCLUDE_NONE -std=c11 -o Alchemaster
	./Alchemaster
