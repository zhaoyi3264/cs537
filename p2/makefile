CC = gcc
WARNING_FLAGS = -Wall -Wextra
LIB_FLAGS = -pthread
EXE = prodcomm

all: main.o queue.o
	$(CC) $(LIB_FLAGS) main.o queue.o -o $(EXE)

main.o: main.c queue.h
	$(CC) $(WARNING_FLAGS) -c main.c

queue.o: queue.c queue.h
	$(CC) $(WARNING_FLAGS) -c queue.c

clean:
	rm $(EXE) *.o
