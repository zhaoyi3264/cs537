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
