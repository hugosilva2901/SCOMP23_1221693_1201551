main: main.o
	gcc main.o -o main -lrt
main.o:	main.c
	gcc -Wall -o main main.c -lrt
run: main
		./main
clean:
		rm *.o main
debug:
		gdb main