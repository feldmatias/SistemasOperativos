#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define STDOUT 1
#define CHARS_LEER 100


void cat0(const char* file) {
    int fd = open(file, O_RDONLY); //Asumo que se abre correctamente
    char buffer[CHARS_LEER];
    ssize_t leidos = 1, escritos;

    while (leidos > 0){
    	leidos = read(fd, buffer, CHARS_LEER);
    	escritos = 0;
    	while (escritos < leidos){
			escritos += write (STDOUT, buffer + escritos, leidos - escritos);
    	}
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    cat0(argv[1]);
}
