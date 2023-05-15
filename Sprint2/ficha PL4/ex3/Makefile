main: main.o
	gcc main.o -o main -lrt -lpthread
main.o:	main.c
	gcc -Wall -g -c main.c -o main.o -lrt -lpthread
run: main
		./main
clean:
		rm *.o main
