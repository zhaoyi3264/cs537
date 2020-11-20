CC = gcc
WARNING_FLAGS = -Wall -Wextra -pedantic
LIB_FLAGS = -lm
EXE = 537pfsim-fifo
SCAN_BUILD_DIR = scan-build-out

all: main.o trace_parser.o process_table.o
	$(CC) -o $(EXE) main.o trace_parser.o process_table.o $(LIB_FLAGS)

main.o: main.c
	$(CC) $(WARNING_FLAGS) -c main.c

trace_parser.o: trace_parser.c trace_parser.h process_table.h
	$(CC) $(WARNING_FLAGS) -c trace_parser.c

process_table.o: process_table.c process_table.h
	$(CC) $(WARNING_FLAGS) -c process_table.c
	
page_table.o: page_table.c page_table.h
	$(CC) $(WARNING_FLAGS) -c page_table.c
	
page_frame_table.o: page_frame_table.c page_frame_table.h
	$(CC) $(WARNING_FLAGS) -c page_frame_table.c

clean:
	rm $(EXE) *.o
	
test: test.c page_table.o
	$(CC) $(WARNING_FLAGS) -c test.c
	$(CC) -o test test.o page_table.o
	
scan-build: clean
	scan-build -o $(SCAN_BUILD_DIR) make

scan-view: scan-build
	firefox -new-window $(SCAN_BUILD_DIR)/*/index.html
