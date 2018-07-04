#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define CHARS_LEER 100


void cp0(const char* file_in, const char* file_out) {
    int fd_in = open(file_in, O_RDONLY); //Asumo que se abre correctamente

    mode_t permisos = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
    int fd_out = open(file_out, O_CREAT|O_WRONLY|O_TRUNC , permisos); //Asumo que se abre correctamente

    char buffer[CHARS_LEER];
    ssize_t leidos = 1, escritos;

    while (leidos > 0){
    	leidos = read(fd_in, buffer, CHARS_LEER);
    	escritos = 0;
    	while (escritos < leidos){
			escritos += write (fd_out, buffer + escritos, leidos - escritos);
    	}
    }
    close(fd_in);
    close(fd_out);
}

int main(int argc, char *argv[]) {
    cp0(argv[1], argv[2]);
}