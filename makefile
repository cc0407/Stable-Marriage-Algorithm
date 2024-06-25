all: q2

q2: Q2.c Q2.h
	gcc -Wpedantic -std=gnu99 Q2.c -g -o q2

clean:
	rm q2
