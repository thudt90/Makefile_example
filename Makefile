edit: main.o add.o sub.o mul.o dev.o print.o
	gcc -o edit main.o add.o sub.o mul.o dev.o print.o

main.o: main.c main.h
	gcc -c main.c
add.o : add.c recipes.h
	gcc -c add.c
sub.o : sub.c recipes.h
	gcc -c sub.c
mul.o : mul.c recipes.h
	gcc -c mul.c
dev.o : dev.c recipes.h
	gcc -c dev.c
print.o : print.c print_data.h main.h
	gcc -c print.c

clean :
	rm edit main.o add.o sub.o mul.o dev.o print.o

