ex12a: main.o
	gcc main.o -o ex12a
	
main.o: main.c
	gcc -g -Wall -c main.c -o main.o

clean:
	rm -f *.o ex12a

run: ex12a
	./ex12a