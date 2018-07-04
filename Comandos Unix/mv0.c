#define _DEFAULT_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


void mv0(const char* file, const char* dest) {
	rename(file, dest);
}

int main(int argc, char *argv[]) {
	mv0(argv[1], argv[2]);
}


/*
Se puede cambiar el nombre del archivo, 
tanto en el mismo directorio como en otro.

./mv0 archivo1 archivo2  ->  renombra el archivo1 a archivo2.

./mv0 archivo1 dir/archivo1  -> mueve el archivo1 pero no lo renombra.

./mv0 archivo1 dir/archivo2  -> mueve el archivo1 y lo renombra.

Ocurre lo mismo para directorios.
*/