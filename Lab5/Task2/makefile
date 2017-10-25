all: task2

task2: task2.o LineParser.o JobControl.o
	gcc -m32 -g -Wall -o task2  task2.o LineParser.o JobControl.o
	
task2.o: task2.c 
	gcc -g -Wall -m32  -c -o task2.o task2.c
	
LineParser.o: LineParser.c LineParser.h
	gcc -g -Wall -m32  -c -o LineParser.o LineParser.c
	
JobControl.o: JobControl.c JobControl.h
	gcc -g -Wall -m32  -c -o JobControl.o JobControl.c
