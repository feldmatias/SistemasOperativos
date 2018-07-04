#include <unistd.h>

void rm0(const char* file) {
	unlink(file);
}

int main(int argc, char *argv[]) {
	rm0(argv[1]);
}
