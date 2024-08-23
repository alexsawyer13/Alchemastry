.PHONY: Alchemaster

Alchemaster:
	gcc src/main.c vendor/glad.c vendor/stb/stb_image.c src/maths.c src/platform.c src/gfx.c src/game.c src/registry.c -Iinclude/all -Ivendor -Llib/gcc -lglfw3 -lGL -lm -D GLFW_INCLUDE_NONE -std=c11 -o Alchemastry
	./Alchemastry
