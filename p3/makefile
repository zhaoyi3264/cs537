CC = gcc
WARNING_FLAGS = -Wall -Wextra -pedantic
EXE = 537make

all: main.o process_control.o text_parsing.o spec_repr.o spec_graph.o
	$(CC) -o $(EXE) main.o process_control.o text_parsing.o spec_repr.o spec_graph.o
	cp 537make proj3/advanced_cycle_testcase
	cp 537make proj3/basic_cycle_testcase
	cp 537make proj3/basic_testcase
	cp 537make proj3/linesize_testcase
	cp 537make proj3/missing_colon
	cp 537make proj3/missing_tab
	cp 537make proj3/modification_check
	cp 537make proj3/null_character
	cp 537make proj3/separate_graph_cycles
	cp 537make proj3/shuffled_cycle_testcase
	cp 537make proj3/simple_testcase
	cp 537make proj3/whitespace_testcase

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
	
test: test.c
	$(CC) $(WARNING_FLAGS) test.c -o out
	./out

clean:
	rm $(EXE) *.o
