memgrind: memgrind.c
all: memgrind.c  
	gcc -g memgrind.c -o memgrind
clean:
	rm -rf memgrind
