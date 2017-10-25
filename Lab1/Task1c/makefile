 
all: encoder

encoder: encoder.o
	gcc -m32 -g -Wall -o encoder encoder.o

encoder.o: encoder.c 
	gcc -g -Wall -m32 -ansi -c -o encoder.o encoder.c