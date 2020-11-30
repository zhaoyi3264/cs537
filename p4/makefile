CC = gcc -pg
WARNING_FLAGS = -Wall -Wextra -pedantic
LIB_FLAGS = -lm
EXE_FIFO = 537pfsim-fifo
EXE_LRU = 537pfsim-lru
EXE_CLOCK = 537pfsim-clock
SCAN_BUILD_DIR = scan-build-out

all: 537pfsim-fifo 537pfsim-lru 537pfsim-clock

537pfsim-fifo: main.o trace_parser.o process_table.o page_table.o page_frame.o schedule_fifo.o disk_queue.o
	$(CC) -o $(EXE_FIFO) main.o trace_parser.o process_table.o page_table.o page_frame.o schedule_fifo.o disk_queue.o $(LIB_FLAGS)

537pfsim-lru: main.o trace_parser.o process_table.o page_table.o page_frame.o schedule_lru.o disk_queue.o
	$(CC) -o $(EXE_LRU) main.o trace_parser.o process_table.o page_table.o page_frame.o schedule_lru.o disk_queue.o $(LIB_FLAGS)

537pfsim-clock: main.o trace_parser.o process_table.o page_table.o page_frame.o schedule_clock.o disk_queue.o
	$(CC) -o $(EXE_CLOCK) main.o trace_parser.o process_table.o page_table.o page_frame.o schedule_clock.o disk_queue.o $(LIB_FLAGS)

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
	
schedule_lru.o: schedule_lru.c schedule_algo.h page_frame.h
	$(CC) $(WARNING_FLAGS) -c schedule_lru.c
	
schedule_clock.o: schedule_clock.c schedule_algo.h page_frame.h
	$(CC) $(WARNING_FLAGS) -c schedule_clock.c
	
disk_queue.o: disk_queue.c disk_queue.h
	$(CC) $(WARNING_FLAGS) -c disk_queue.c

clean:
	rm -f $(EXE_FIFO) $(EXE_LRU) $(EXE_CLOCK) *.o
	
test: test.c process_table.o page_table.o page_frame.o schedule_fifo.o disk_queue.o
	$(CC) $(WARNING_FLAGS) -c test.c
	$(CC) -o test test.o process_table.o page_table.o page_frame.o schedule_fifo.o disk_queue.o
	
scan-build: clean
	scan-build -o $(SCAN_BUILD_DIR) make

scan-view: scan-build
	firefox -new-window $(SCAN_BUILD_DIR)/*/index.html
