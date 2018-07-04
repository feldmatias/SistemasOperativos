#define _DEFAULT_SOURCE
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <dirent.h>
#include <stdbool.h>

void ls0() {
	DIR* dir = opendir("./");
	DIR* directorio = dir;
	while (true){
		struct dirent* entrada = readdir(directorio);
		if (!entrada){
			break;
		}
		printf("%s\n", entrada -> d_name);
	}
	closedir(dir);
}

int main(int argc, char *argv[]) {
    ls0();
}