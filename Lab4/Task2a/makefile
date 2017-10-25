 
all: t2a

t2a: start.o util.o t2a.o
	ld -m elf_i386 start.o t2a.o util.o -o t2a

start.o: start.s
	nasm -f elf start.s -o start.o

util.o: util.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o

t2a.o: t2a.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector t2a.c -o t2a.o