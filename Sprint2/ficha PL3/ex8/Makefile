main: main.o
	gcc main.o -o main -lrt
main.o:	main.c
	gcc -Wall -g -c main.c -o main.o -lrt
run: main
		./main
clean:
		rm *.o main
debug:
		gdb main