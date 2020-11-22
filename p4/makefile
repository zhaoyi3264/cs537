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
	
page_frame.o: page_frame.c page_frame.h
	$(CC) $(WARNING_FLAGS) -c page_frame.c
	
schedule_fifo.o: schedule_fifo.c schedule_algo.h page_frame.h
	$(CC) $(WARNING_FLAGS) -c schedule_fifo.c
	
disk_queue.o: disk_queue.c disk_queue.h
	$(CC) $(WARNING_FLAGS) -c disk_queue.c

clean:
	rm $(EXE) *.o
	
test: test.c process_table.o page_table.o page_frame.o schedule_fifo.o disk_queue.o
	$(CC) $(WARNING_FLAGS) -c test.c
	$(CC) -o test test.o process_table.o page_table.o page_frame.o schedule_fifo.o disk_queue.o
	
scan-build: clean
	scan-build -o $(SCAN_BUILD_DIR) make

scan-view: scan-build
	firefox -new-window $(SCAN_BUILD_DIR)/*/index.html
