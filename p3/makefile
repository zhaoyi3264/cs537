CC = gcc
WARNING_FLAGS = -Wall -Wextra -pedantic
EXE = 537make

all: main.o text_parsing.o spec_repr.o spec_graph.o
	$(CC) -o $(EXE) main.o text_parsing.o spec_repr.o spec_graph.o

main.o: main.c text_parsing.h
	$(CC) $(WARNING_FLAGS) -c main.c

text_parsing.o: text_parsing.c text_parsing.h spec_repr.h spec_graph.h
	$(CC) $(WARNING_FLAGS) -c text_parsing.c

spec_repr.o: spec_repr.c spec_repr.h
	$(CC) $(WARNING_FLAGS) -c spec_repr.c

spec_graph.o: spec_graph.c spec_graph.h spec_repr.h
	$(CC) $(WARNING_FLAGS) -c spec_graph.c

clean:
	rm $(EXE) *.o
