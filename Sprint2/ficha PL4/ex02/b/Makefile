main: main.o
	gcc main.o -o main -lpthread -lrt
main.o:	main.c
	gcc -Wall -g -c main.c -o main.o -lpthread 		-lrt
run: main
		./main
clean:
		rm *.o main
