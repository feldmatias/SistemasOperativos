#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

void rm1(const char* file) {
	int res = unlink(file);

	if (res == -1){
		char* formato = "rm: cannot remove '%s'";
		char buffer[strlen(formato) + strlen(file) + 1];
		sprintf(buffer, formato, file);
		perror(buffer);
	}
}

int main(int argc, char *argv[]) {
	rm1(argv[1]);
}

/*
En este caso no uso explicitamente el valor de errno ya que la funcion perror() lo usa internamente.
perror(mensaje)  imprime el mensaje seguido del error. 
El error lo sabe por el valor de errno.
(Asumiendo que sprintf no genera un nuevo error).
*/

