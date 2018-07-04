#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void touch0(const char* file) {
	mode_t permisos = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
	int fd = open(file, O_CREAT, permisos);
	close(fd);
}

int main(int argc, char *argv[]) {
	touch0(argv[1]);
}
