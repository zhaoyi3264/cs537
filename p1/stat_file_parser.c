/*
 * Provides methods for parsing and getting fields in stat, statm, cmdline files
 * 
 * Authors: 
 * - Zhaoyi Zhang, netid: zzhang825
 * - Richard Li, netid: tli354
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include "common.h"
#include "stat_file_parser.h"

/*
 * Read the pid, status, utime, stime fields in /proc/[pid]/stat into a Stat
 * struct
 * 
 * file: the path of the stat file of a process
 * stat: the struct to store parsed fields
 */
void parse_stat(char *file, struct Stat *stat) {
	FILE *fp = open_file(file);
	char s[64];
	int d = 0;
	// skips to the parameters we need
	int r = fscanf(fp, "%d %s %c %d %d %d %d %d %d %d %d %d %d %lu %lu",
		&(stat->pid), s, &(stat->state),
		&d, &d, &d, &d, &d, &d, &d, &d, &d, &d,
		&(stat->utime), &(stat->stime));
	if (r < 0) {
		exit(1);
	}
	close_file(fp);
}

/*
 * Read the Vmsize field in /proc/[pid]/statm
 * 
 * file: the path of the statm file of a process
 * 
 * return: the Vmsize field
 */
int parse_statm(char *file) {
	FILE *fp = open_file(file);
	int d = 0;
	if (fscanf(fp, "%d", &d) < 0) {
		exit(1);
	}
	close_file(fp);
	return d;
}
/*
 * Read the command that starts the process in /proc/[pid]/cmdline
 * 
 * file: the path of the cmdline file of a process
 * cmd: the buffer to store the command string
 */
void get_cmd(char *file, char *cmd) {
	int i = 0;
	FILE *fp = open_file(file);
	do {
		int c = fgetc(fp);
		if(feof(fp)) {
			break;
		}
		cmd[i++] = c;
	} while(1);
	close_file(fp);
}

/*
 * Read the memory content of the process
 * 
 * pid: the pid of the process
 * buf: the buffer to read memory into
 * addr: addr of the memory to read
 * size: size in bytes of the memory to read
 */
void get_mem(char *pid, char *buf, long int addr, int size) {
	// get file names
	char *mem_file = malloc(32);
	if (mem_file == NULL) {
		exit(1);
	}
	if (sprintf(mem_file, "/proc/%s/mem", pid) < 0) {
		exit(1);
	}
	char *maps_file = malloc(32);
	if (maps_file == NULL) {
		exit(1);
	}
	if (snprintf(maps_file, 32, "/proc/%s/maps", pid) < 0) {
		exit(1);
	}
	// check if the addr is a valid address
	FILE *fp = open_file(maps_file);
	char line[256];
	int valid = 0;
	while (fgets(line, sizeof(line), fp)) {
		// get the lower and upper bound of the valid address
		char *bound = strtok(line, " ");
		char *lower = strtok(bound, "-");
		char *upper = strtok(NULL, "-");
		long int addr_l = strtol(lower, NULL, 16);
		long int addr_u = strtol(upper, NULL, 16);
		//~ printf("%ld %ld\n", addr_l, addr_u);
		if (addr_l <= addr && addr <= addr_u) {
			valid = 1;
			break;
		}
	}
	close_file(fp);
	if (!valid) {
		printf("error: invalid memory address\n");
		exit(1);
	}
	// read the memory using ptrace and pread
	int pid_num = atoi(pid);
	if (ptrace(PTRACE_ATTACH, pid_num, NULL, NULL) == -1) {
		exit(1);
	}
	if (waitpid(pid_num, NULL, 0) == -1) {
		exit(1);
	}
	int mem_fd = open(mem_file, O_RDONLY);
	if (mem_fd == -1) {
		exit(1);
	}
	if (pread(mem_fd, buf, size, addr) < 0) {
		exit(1);
	}
	if (ptrace(PTRACE_DETACH, pid_num, NULL, NULL) == -1) {
		exit(1);
	}
	if (close(mem_fd) != 0) {
		exit(1);
	}
}

/*
 * Opens a file pointer
 * 
 * file: the file name
 * 
 * return: the file pointer
 */
FILE *open_file(char *file) {
	FILE *fp = fopen(file, "r");
	if (fp == NULL) {
		exit(1);
	}
	return fp;
}

/*
 * Closes a file pointer
 * 
 * fp: the file pointer
 */
void close_file(FILE *fp) {
	if (fclose(fp) != 0) {
		exit(1);
	}
}
