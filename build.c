#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#define CMD_LEN_MAX 10240
#define STRING_LEN_MAX 512
#define STRING_COUNT_MAX 256

typedef struct
{
	char path[STRING_LEN_MAX];
} String;

typedef struct
{
	String files[STRING_COUNT_MAX];
	int       file_count;
} String_List;

int is_file_c(const struct dirent *dir)
{
	int length;
	
	// Check it's a regular file
	if (dir->d_type != DT_REG)
		return 0;

	length = strlen(dir->d_name);
	if (length < 3)
		return 0;

	if (dir->d_name[length-1] == 'c' && dir->d_name[length-2] == '.')
		return 1;

	return 0;
}

int is_directory(const struct dirent *dir)
{
	if (dir->d_type != DT_DIR)
		return 0;

	if (strlen(dir->d_name) == 1 && dir->d_name[0] == '.')
		return 0;

	if (strlen(dir->d_name) == 2 && dir->d_name[0] == '.' && dir->d_name[1] == '.')
		return 0;

	return 1;
}

// TODO: ASSUMES THAT NO PATH GOES OVER 512 CHARACTERS
// TODO: Leaks all memory from scandir
int get_sources_from_dir(String_List *list, const char *directory)
{
	struct dirent **files;
	int             num_files;
	char		sub_dir[STRING_LEN_MAX];

	// Add all sources straight inside the directory

	num_files = scandir(directory, &files, is_file_c, alphasort);
	if (num_files == -1)
	{
		fprintf(stderr, "scandir failed to find files in directory %s\n", directory);
		return 0;
	}

	for (int i = 0; i < num_files; i++)
	{
		if (list->file_count >= STRING_COUNT_MAX)
		{
			fprintf(stderr, "filled up file list while finding sources in directory %s, ruh roh\n", directory);
			return 0;
		}
		sprintf(list->files[list->file_count++].path, "%s/%s", directory, files[i]->d_name);
	}

	// Recursively look inside subdirectories to find more!

	num_files = scandir(directory, &files, is_directory, alphasort);

	for (int i = 0; i < num_files; i++)
	{
		char sub_dir[STRING_LEN_MAX];
		sprintf(sub_dir, "%s/%s", directory, files[i]->d_name);
		get_sources_from_dir(list, sub_dir);
	}

	return 1;
}

int get_compile_flags(String_List *list)
{
	FILE *file;
	int  length;

	file = fopen("compile_flags.txt", "r");
	if (!file)
	{
		fprintf(stderr, "Cannot open compile_flags.txt\n");
		return 0;
	}
	
	fseek(file, 0, SEEK_END);
	length = (int)ftell(file);
	fseek(file, 0, SEEK_SET);

	while(!feof(file))
	{
		char line[STRING_LEN_MAX];
		int bytes_to_copy;

		if (fgets(line, STRING_LEN_MAX, file))
		{
			if (list->file_count >= STRING_COUNT_MAX)
			{
				fprintf(stderr, "filled up file list while reading in compile flags, ruh roh!\n");
				return 0;
			}

			bytes_to_copy = strlen(line);
			if (bytes_to_copy == 0)
				break;
			if (line[bytes_to_copy - 1] == '\n')
				bytes_to_copy -= 1;
			
			memcpy(list->files[list->file_count++].path, line, bytes_to_copy);
		}
	}

	fclose(file);
	return 1;
}

int main()
{
	char cmd[CMD_LEN_MAX];
	int cmd_len;

	String_List sources;
	String_List compile_flags;

	sources.file_count = 0;
	compile_flags.file_count = 0;
	cmd_len = 0;

	cmd_len += sprintf(cmd, "gcc");

	// Get source files

	if (!get_sources_from_dir(&sources, "src")) return -1;
	if (!get_sources_from_dir(&sources, "vendor")) return -1;

	for (int i = 0; i < sources.file_count; i++)
	{
		printf("Adding source: %s\n", sources.files[i].path);
		cmd_len += sprintf(cmd + cmd_len, " %s", sources.files[i].path);
	}

	// Read compile flags from file
	
	if (!get_compile_flags(&compile_flags)) return -1;

	for (int i = 0; i < compile_flags.file_count; i++)
	{
		printf("Adding compile flag: %s\n", compile_flags.files[i].path);
		cmd_len += sprintf(cmd + cmd_len, " %s", compile_flags.files[i].path);
	}

	// Recreate build directory
	
	printf("Creating directory: build\n");
	system("rm -rf build");
	system("mkdir -p build");

	// Run compile command

	printf("Compile command: %s\n\n", cmd);
	system(cmd);

	// Run game
	
	system("build/game.o");

	return 0;
}
