#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "stat_file_parser.h"

/*
 * Parse parameters from stat
 * (1) pid
 * (3) state
 * (14) utime
 * (15) stime
 */
void parse_stat(char *file, struct Stat *stat) {
	check_file_name(file, "stat");
	FILE *fp = open_file(file);
	if (fp == NULL) {
		fprintf(stderr, "Error opening file %s\n", file);
		exit(1);
	}
	char s[50];
	int d = 0;
	/* skips to the parameters we need */
	fscanf(fp, "%d %s %c %d %d %d %d %d %d %d %d %d %d %d %d",
		&(stat->pid), s, &(stat->state),
		&d, &d, &d, &d, &d, &d, &d, &d, &d, &d,
		&(stat->utime), &(stat->stime));
	close_file(file, fp);
}

/*
 * Parse size in statm file
 * (1) size
 */
int parse_statm(char *file) {
	check_file_name(file, "statm");
	FILE *fp = open_file(file);
	int d = 0;
	fscanf(fp, "%d", &d);
	close_file(file, fp);
	return d;
}
/*
 * Parse file name from command
 */
void get_cmd(char *file, char *cmd) {
	check_file_name(file, "cmdline");
	FILE *fp = open_file(file);
	fscanf(fp, "%s", cmd);
	close_file(file, fp);
}

/*
 * Opens file and checks if successful
 */
FILE *open_file(char *file) {
	FILE *fp = fopen(file, "r");
	if (fp == NULL) {
		fprintf(stderr, "Error opening file %s\n", file);
		exit(1);
	}
	return fp;
}

/*
 * Closes file and checks if successful
 */
void close_file(char *file, FILE *fp) {
	if (fclose(fp) != 0) {
		fprintf(stderr, "Error closing file %s\n", file);
		exit(1);
	}
}

/*
 * Checks if the file name is valid 
 * TODO: May delete it
 */
void check_file_name(char *file, char *target) {
    size_t file_len = strlen(file);
    size_t target_len = strlen(target);
	char* s = file + file_len - target_len;
	if (s == NULL || strcmp(s, target)) {
		fprintf(stderr,
			"Error in file name %s, when a %s file is expected\n",
			file, target);
			exit(1);
	}
}
