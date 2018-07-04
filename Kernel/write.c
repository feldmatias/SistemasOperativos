#include "decls.h"
#include "multiboot.h"

#define COLUMNS 80
#define ROWS 25

#define VGABUF ((volatile char *) 0xB8000)

void vga_write(const char *s, int8_t linea, uint8_t color) {
	if (linea < 0) {
		linea = ROWS + linea;
	}
	volatile char* buff = VGABUF + linea * COLUMNS * 2;
	while (*s != '\0') {
		*buff++ = *s++;
		*buff++ = color;
	}
}


bool fmt_int(uint64_t val, char *s, size_t bufsize){
	uint64_t digits = 0;
	uint64_t aux = val;
	while (aux > 0){
		digits++;
		aux /= 10;
	}

	if (digits >= bufsize){  //>= para agregar el \0
		return false;
	}

	for (int i = digits - 1; i >= 0; i--){
		s[i] = val % 10 + '0';
		val /= 10;
	}
	s[bufsize - 1] = '\0';
	return true;
}

void __attribute__((regparm(2)))
vga_write_cyan(const char *s, int8_t linea) {
    vga_write(s, linea, 0xB0);
}


