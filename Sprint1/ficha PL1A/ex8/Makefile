ex8: main.o
	gcc main.o -o ex8
	
main.o: main.c
	gcc -g -Wall -c main.c -o main.o

clean:
	rm -f *.o ex8

run: ex8
	./ex8