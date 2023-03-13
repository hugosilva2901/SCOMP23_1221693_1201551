ex12b: main.o
	gcc main.o -o ex12b
	
main.o: main.c
	gcc -g -Wall -c main.c -o main.o

clean:
	rm -f *.o ex12b

run: ex12b
	./ex12b