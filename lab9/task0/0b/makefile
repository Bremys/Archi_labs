all: task0b
 
task0b: task0b.o
	ld -m elf_i386 task0b.o -o task0b

task0b.o: skeleton.s
	nasm -f elf skeleton.s -o task0b.o

	
clean: 
	rm -f *.o task0b