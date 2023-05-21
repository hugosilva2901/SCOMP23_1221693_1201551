main: main.o
	gcc main.o -o main -lpthread
main.o:	main.c
	gcc -Wall -g -c main.c -o main.o -lpthread
run: main
		./main
clean:
		rm *.o main
debug:
		gdb mains
# commit fix