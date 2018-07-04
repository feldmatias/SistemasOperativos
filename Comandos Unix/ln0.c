#define _DEFAULT_SOURCE
#include <unistd.h>

void ln0(const char* file, const char* enlace) {
	symlink(file, enlace);
}

int main(int argc, char *argv[]) {
	ln0(argv[1], argv[2]);
}


/*
Si se crea un link a un archivo que no existe, el link se crea normalmente, 
pero cuando se quiera acceder al mismo será como abrir un archivo que no existe.
Es decir, el link no sirve para nada porque apunta a un archivo que no existe.

Ocurre lo mismo si se crea el link y luego se borra el archivo original.
*/