ex5: main.o
	gcc main.o -o ex5
	
main.o: main.c
	gcc -g -Wall -c main.c -o main.o

clean:
	rm -f *.o ex5

run: ex5
	./ex5