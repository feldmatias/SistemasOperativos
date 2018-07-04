#define _DEFAULT_SOURCE
#include <unistd.h>

void ln1(const char* file, const char* enlace) {
	link(file, enlace);
}

int main(int argc, char *argv[]) {
	ln1(argv[1], argv[2]);
}


/*
Un soft link es como crear un acceso directo del archivo. 
Funciona siempre y cuando exista el archivo original.
Si se borra el archivo original será como abrir un archivo que no existe.

En cambio un hard link es crear otro nombre para el archivo.
Ambos nombres acceden al mismo archivo.
El archivo no se copia, ni se mueve ni se duplica.
Si se modifica el archivo desde un nombre, cambia el otro tambien. (Porque es el mismo)
Si se borra el original, el archivo sigue existiendo con el otro nombre.
El archivo únicamente se elimina del disco cuando se borran todos los hard links del mismo.
(Un hard link implica agregar otro inode que apunta al mismo datablock).
*/