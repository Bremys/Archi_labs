 
all: task2c

task2c: start.o util.o task2c.o
	ld -m elf_i386 start.o task2c.o util.o -o task2c

start.o: start.s
	nasm -f elf start.s -o start.o

util.o: util.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o

task2c.o: task2c.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector task2c.c -o task2c.o