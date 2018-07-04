#define _DEFAULT_SOURCE
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <stdbool.h>

#define CHARS_LEER sysconf(_SC_PAGE_SIZE)


void cp1(const char* file_in, const char* file_out) {
    int fd_in = open(file_in, O_RDWR); //Asumo que se abre correctamente
    
    mode_t permisos = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
    int fd_out = open(file_out, O_CREAT|O_RDWR|O_TRUNC , permisos); //Asumo que se abre correctamente

    struct stat datos;
    stat(file_in, &datos);
    ftruncate(fd_out, datos.st_size); //Agrando el archivo de salida, sino mmap tira segmentation fault
    

    int i = 0;
    bool termino = false;
    while (!termino){
        off_t offset = i * CHARS_LEER;
        int cant = CHARS_LEER;
        if (offset + cant > datos.st_size){
            cant = datos.st_size - offset;
            termino = true;
        }
        
        char* add_in = mmap(NULL, cant, PROT_READ, MAP_SHARED, fd_in, offset);
        char* add_out = mmap(NULL, cant, PROT_WRITE, MAP_SHARED, fd_out, offset);
        memcpy(add_out, add_in, cant);
        munmap(add_in, cant);
        munmap(add_out, cant);

        i++;
    }

    
    close(fd_in);
    close(fd_out);
}

int main(int argc, char *argv[]) {
    cp1(argv[1], argv[2]);
}