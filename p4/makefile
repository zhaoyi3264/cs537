CC = gcc
WARNING_FLAGS = -Wall -Wextra -pedantic
LIB_FLAGS = -lm
EXE = 537pfsim
SCAN_BUILD_DIR = scan-build-out

all: main.o trace_parser.o process_table.o
	$(CC) -o $(EXE) main.o trace_parser.o process_table.o $(LIB_FLAGS)

main.o: main.c
	$(CC) $(WARNING_FLAGS) -c main.c

trace_parser.o: trace_parser.c process_table.h
	$(CC) $(WARNING_FLAGS) -c trace_parser.c

process_table.o: process_table.c
	$(CC) $(WARNING_FLAGS) -c process_table.c

clean:
	rm $(EXE) *.o

scan-build: clean
	scan-build -o $(SCAN_BUILD_DIR) make

scan-view: scan-build
	firefox -new-window $(SCAN_BUILD_DIR)/*/index.html
