# Target library
lib := libuthread.a
CC = gcc
CFLAGS = -Wall -Wextra -Werror
OBJ = queue.o uthread.o sem.o context.o preempt.o
SAL = ar rcs
all: $(lib) 

## generate queue.o
queue.o: queue.c
	$(CC) $(CFLAGS) -c -o queue.o queue.c

uthread.o: uthread.c
	$(CC) $(CFLAGS) -c -o uthread.o uthread.c

context.o: context.c
	$(CC) $(CFLAGS) -c -o context.o context.c

sem.o: sem.c
	$(CC) $(CFLAGS) -c -o sem.o sem.c

preempt.o: preempt.c
	$(CC) $(CFLAGS) -c -o preempt.o preempt.c

libuthread.a: queue.o sem.o uthread.o context.o preempt.o
	$(SAL) libuthread.a $(OBJ)

clean:
	rm -f *.o $(lib)

## TODO: Phase 1
