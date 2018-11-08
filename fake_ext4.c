#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static const char *lib_name = "libfake_ext4.so";

int main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "Error: no executable file specified\n");
		fprintf(stderr, "usage: %s <program> [<args>...]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// TODO: append? somehow allow chaining LD_PRELOADS?
	// TODO: determine the full path for this lib when preloading

	// my path
	char *my_path = strdup(argv[0]);

	// remove the last component
	char *last_slash = strrchr(my_path, '/');
	*last_slash = '\0';

	// append the library path
	char *np = realloc(my_path, strlen(my_path) + 1 + strlen(lib_name) + 1);
	if (!np) {
		fprintf(stderr, "Error: path allocation failure: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	my_path = np;
	strcat(my_path, "/");
	strcat(my_path, lib_name);

	fprintf(stderr, "Info: using preload: %s\n", my_path);

	setenv("LD_PRELOAD", my_path, 1);

	int r = execvp(argv[1], argv + 1);
	fprintf(stderr, "Error: execvp of %s failed: %d: %d: %s\n",
			argv[1], r, errno, strerror(errno));
	return 0;
}
