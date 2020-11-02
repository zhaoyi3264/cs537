CC = gcc
WARNING_FLAGS = -Wall -Wextra -pedantic
EXE = 537make
SCAN_BUILD_DIR = scan-build-out

all: main.o process_control.o text_parsing.o spec_repr.o spec_graph.o
	$(CC) -o $(EXE) main.o process_control.o text_parsing.o spec_repr.o spec_graph.o

main.o: main.c text_parsing.h
	$(CC) $(WARNING_FLAGS) -c main.c

process_control.o: process_control.c process_control.h spec_graph.h
	$(CC) $(WARNING_FLAGS) -c process_control.c

text_parsing.o: text_parsing.c text_parsing.h spec_graph.h
	$(CC) $(WARNING_FLAGS) -c text_parsing.c

spec_graph.o: spec_graph.c spec_graph.h spec_repr.h
	$(CC) $(WARNING_FLAGS) -c spec_graph.c

spec_repr.o: spec_repr.c spec_repr.h
	$(CC) $(WARNING_FLAGS) -c spec_repr.c

clean:
	rm $(EXE) *.o

scan-build: clean
	scan-build -o $(SCAN_BUILD_DIR) make

scan-view: scan-build
	firefox -new-window $(SCAN_BUILD_DIR)/*/index.html
