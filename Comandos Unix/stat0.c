#define _DEFAULT_SOURCE
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

void stat0(const char* file) {
	struct stat datos;
	stat(file, &datos);
	char* tipo;
	switch (datos.st_mode & S_IFMT){
		case S_IFREG:
			tipo = "regular file";
			break;
		case S_IFDIR:
			tipo = "directory";
			break;
	}

	printf("Size: %ld\nFile: %s\nType: %s\n", datos.st_size, file, tipo);
}

int main(int argc, char *argv[]) {
    stat0(argv[1]);
}
