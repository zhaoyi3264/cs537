CC = gcc
WARNING_FLAGS = -Wall -Wextra -pedantic
LIB_FLAGS = -lm
EXE_FIFO = 537pfsim-fifo
EXE_LRU = 537pfsim-lru
EXE_CLOCK = 537pfsim-clock
EXE_OPT = 537pfsim-opt
DEP = trace_parser.o process_table.o page_table.o page_frame.o disk_queue.o statistics.o

all: 537pfsim-fifo 537pfsim-lru 537pfsim-clock 537pfsim-opt

537pfsim-fifo: main.o $(DEP) schedule_fifo.o
	$(CC) -o $(EXE_FIFO) main.o $(DEP) schedule_fifo.o $(LIB_FLAGS)

537pfsim-lru: main.o $(DEP) schedule_lru.o
	$(CC) -o $(EXE_LRU) main.o $(DEP) schedule_lru.o $(LIB_FLAGS)

537pfsim-clock: main.o $(DEP) schedule_clock.o
	$(CC) -o $(EXE_CLOCK) main.o $(DEP) schedule_clock.o $(LIB_FLAGS)

537pfsim-opt: main.o $(DEP) schedule_opt.o
	$(CC) -o $(EXE_OPT) main.o $(DEP) schedule_opt.o $(LIB_FLAGS)

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
	
disk_queue.o: disk_queue.c disk_queue.h
	$(CC) $(WARNING_FLAGS) -c disk_queue.c
	
statistics.o: statistics.c statistics.h
	$(CC) $(WARNING_FLAGS) -c statistics.c
	
schedule_fifo.o: schedule_fifo.c schedule_algo.h page_frame.h
	$(CC) $(WARNING_FLAGS) -c schedule_fifo.c
	
schedule_lru.o: schedule_lru.c schedule_algo.h page_frame.h
	$(CC) $(WARNING_FLAGS) -c schedule_lru.c
	
schedule_clock.o: schedule_clock.c schedule_algo.h page_frame.h
	$(CC) $(WARNING_FLAGS) -c schedule_clock.c
	
schedule_opt.o: schedule_opt.c schedule_algo.h page_frame.h
	$(CC) $(WARNING_FLAGS) -c schedule_opt.c

clean:
	rm -f $(EXE_FIFO) $(EXE_LRU) $(EXE_CLOCK) $(EXE_OPT) *.o
