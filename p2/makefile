CC = gcc
WARNING_FLAGS = -Wall -Wextra
LIB_FLAGS = -pthread
EXE = prodcom
SCAN_BUILD_DIR = scan-build-out

all: main.o threads.o statistics.o queue.o
	$(CC) $(WARNING_FLAGS) $(LIB_FLAGS) main.o threads.o statistics.o queue.o -o $(EXE)

main.o: main.c queue.h threads.h
	$(CC) $(WARNING_FLAGS) -c main.c

queue.o: queue.c queue.h statistics.h
	$(CC) $(WARNING_FLAGS) -c queue.c

threads.o: threads.c queue.h
	$(CC) $(WARNING_FLAGS) -c threads.c
	
statistics.o: statistics.c statistics.h
	$(CC) $(WARNING_FLAGS) -c statistics.c

clean:
	rm -f $(EXE) *.o
	rm -rf $(SCAN_BUILD_DIR)
	
scan-build: clean
	scan-build -o $(SCAN_BUILD_DIR) make

scan-view: scan-build
	firefox -new-window $(SCAN_BUILD_DIR)/*/index.html 
