main: main.o
	gcc main.o -o main -lpthread -lrt
main.o:	main.c
	gcc -Wall -g -c main.c -o main.o 
run: main
		./main
clean:
		rm *.o main
debug:
		gdb main
# commit fix