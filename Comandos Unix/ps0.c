#define _DEFAULT_SOURCE
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <dirent.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define CHARS_LEER 100

//Devuelve true si nombre es un numero
bool esProceso(const char* nombre){
	int len = strlen(nombre);
	for(int i = 0; i < len; i++){
		if (!isdigit(nombre[i])){
			return false;
		}
	}
	return true;
}

void ps0() {
	DIR* dir = opendir("/proc");
	DIR* directorio = dir;
	while (true){
		struct dirent* proceso = readdir(directorio);
		if (!proceso){
			break;
		}
		if (esProceso(proceso -> d_name)){
			int len_proceso = strlen(proceso -> d_name);

			//Analizo el archivo comm
			char path[len_proceso + strlen("/proc/") + strlen("/comm") + 1];
			sprintf(path, "/proc/%s/comm", proceso->d_name);
			int fd = open(path, O_RDONLY);

			for (int i = 0; i < 6 - len_proceso; i++){
				//Agrego espacios para encolumnar los pid
				printf(" ");
			}
			printf("%s  ", proceso -> d_name);

			char buffer_lectura[CHARS_LEER + 1];
			ssize_t leidos = 1;

			while (leidos > 0){
				leidos = read(fd, buffer_lectura, CHARS_LEER);
				if (leidos > 0 && buffer_lectura[leidos - 1] == '\n'){
					buffer_lectura[leidos - 1] = '\0'; //Elimino el ultimo \n
				}
				buffer_lectura[leidos] = '\0';
				printf("%s", buffer_lectura);
			}
			close(fd);

			//Analizo el archivo stat
			sprintf(path, "/proc/%s/stat", proceso->d_name);
			FILE* archivo = fopen(path, "r"); //Uso fopen para simplificar el fscanf

			//Muestro algunos datos adicionales del proceso
			char state;
			int ppid, pgrp, session;
			fscanf(archivo, "%*d %*s %c %d %d %d", &state, &ppid, &pgrp, &session);
			printf("  ---  state: %c ppid: %d pgrp: %d session: %d \n", state, ppid, pgrp, session);
			fclose(archivo);

		}
	}
	closedir(dir);
}

int main(int argc, char *argv[]) {
    ps0();
}

/*
En este caso, el analisis del archivo comm podria evitarse, 
ya que el archivo stat incluye la informacion del comm.
Igualmente lo dejo para cumplir con lo que pide la consigna.

Asi se ve una fila tras correr el comando:

3273  ps0  ---  state: R ppid: 2760 pgrp: 3273 session: 2760 
*/