CC = gcc
WARNING_FLAGS = -Wall -Wextra
EXE = 537ps

all: main.o options_processing.o process_list.o stat_file_parser.o
	$(CC) -o $(EXE) main.o options_processing.o process_list.o stat_file_parser.o

main.o: main.c options_processing.h process_list.h stat_file_parser.h struct.h
	$(CC) $(WARNING_FLAGS) -c main.c

options_procssing.o: options_processing.c options_processing.h struct.h
	$(CC) $(WARNING_FLAGS) -c options_processing.c

process_list.o: process_list.c process_list.h struct.h
	$(CC) $(WARNING_FLAGS) -c process_list.c

stat_file_parser.o: stat_file_parser.c stat_file_parser.h struct.h
	$(CC) $(WARNING_FLAGS) -c stat_file_parser.c

clean:
	rm $(EXE) *.o
