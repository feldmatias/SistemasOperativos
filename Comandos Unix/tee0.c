#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define STDIN 0
#define STDOUT 1
#define CHARS_LEER 100


void tee0(const char* file) {
    
    mode_t permisos = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
    int fd = open(file, O_CREAT|O_WRONLY|O_TRUNC , permisos); //Asumo que se abre correctamente

    char buffer[CHARS_LEER];
    ssize_t leidos = 1, escritos;

    while (leidos > 0){
    	leidos = read(STDIN, buffer, CHARS_LEER);
    	escritos = 0;
    	while (escritos < leidos){
			escritos += write (fd, buffer + escritos, leidos - escritos);
    	}
        escritos = 0;
        while (escritos < leidos){
            escritos += write (STDOUT, buffer + escritos, leidos - escritos);
        }
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    tee0(argv[1]);
}