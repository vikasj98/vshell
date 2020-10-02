#19 Aug 2019
#VK
#Makefile for vshell

vshell : main.o vshell.o
	gcc -o vshell main.o vshell.o
	
main.o : main.c vshell.h
	gcc -c main.c

vshell.o : vshell.c vshell.h
	gcc -c vshell.c

clean :
	rm *.o
	

