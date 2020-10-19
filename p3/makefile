CC = gcc
WARNING_FLAGS = -Wall -Wextra
EXE = 537make

all: main.o text_parsing.o
	$(CC) -o $(EXE) main.o text_parsing.o

main.o: main.c text_parsing.h
	$(CC) $(WARNING_FLAGS) -c main.c

text_parsing.o: text_parsing.c text_parsing.h
	$(CC) $(WARNING_FLAGS) -c text_parsing.c

clean:
	rm $(EXE) *.o
