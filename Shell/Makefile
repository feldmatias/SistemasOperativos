CFLAGS := -std=c99 -g -Wall
CC := gcc
BIN := $(filter-out sh.c, $(wildcard *.c))
BINFILES := $(BIN:.c=.o)

all: sh

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

sh: $(BINFILES) sh.c
	$(CC) $(CFLAGS) $^ -o $@

run: all
	./sh

valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all ./sh

clean:
	rm -f sh *.o *.asm

pdf_all:
	a2ps *.h *.c -Av --header="[75.08] Sistemas Operativos" --footer="Padron 99170 Lab Shell" --line-numbers=1 --borders=yes --columns=1  --portrait --output=out.ps --encoding=ISO-8859-1 --tabsize=4 --major=columns --toc
	ps2pdf out.ps shell.pdf
	rm out.ps

pdf_cambios:
	a2ps defs.h types.h builtin.c exec.h exec.c parsing.c runcmd.c sh.c -Av --header="[75.08] Sistemas Operativos" --footer="Padron 99170 Lab Shell" --line-numbers=1 --borders=yes --columns=1  --portrait --output=out.ps --encoding=ISO-8859-1 --tabsize=4 --major=columns --toc
	ps2pdf out.ps shell_solo_cambios.pdf
	rm out.ps

pdf: pdf_all pdf_cambios

.PHONY: clean run
