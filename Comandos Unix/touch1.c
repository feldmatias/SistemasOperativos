#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <utime.h>

void touch1(const char* file) {
	mode_t permisos = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
	int fd = open(file, O_CREAT, permisos);
	close(fd);

	utime(file, NULL);
}

int main(int argc, char *argv[]) {
	touch1(argv[1]);
}