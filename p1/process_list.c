/*
 * Authors: 
 * Zhaoyi Zhang, netid: zzhang825
 * Richard Li, netid: tli354
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include "process_list.h"
#include "struct.h"

/*
 * Gets the uid of a proc
 */
char *get_proc_uid(char *file) {
	FILE *fp = fopen(file, "r");
	if (fp == NULL) {
		fprintf(stderr, "Error opening file %s\n", file);
		exit(1);
	}
	char line[256];
	char temp[4];
	char *uid = malloc(sizeof(char) * 10);
	while(fgets(line, sizeof(line), fp)) {
		if (!strncmp(line, "Uid:", 4)) {
			sscanf(line, "%s\t%s", temp, uid);
		}
	}
	if (fclose(fp) != 0) {
		fprintf(stderr, "Error closing file %s\n", file);
		exit(1);
	}
	return uid;
}

/*
 * Get uid of the user
 */
char *get_user_uid() {
	return get_proc_uid("/proc/self/status");
}

/*
 * Checks if it is a valid pid
 */
int is_pid(char *dir_name) {
	int pid = 1;
	for (int i = 0; dir_name[i] != '\0'; i++) {
		if (!isdigit(dir_name[i])) {
			pid = 0;
			break;
		}
	}
	return pid;
}

/*
 * Checks if a proc belongs to the user
 */
int is_user_proc(char *uid, char *pid) {
	char file[50] = "/proc/";
	char *proc_uid = get_proc_uid(strcat(strcat(file, pid), "/status"));
	return !strcmp(uid, proc_uid);
}

/*
 * Gets a list of procs from the linkedlist we create
 */
struct PNode *get_proc(int user) {
	DIR *dir;
	struct dirent *entry;
	if ((dir = opendir("/proc")) == NULL) {
		fprintf(stderr, "Error reading /proc directory");
		exit(1);
	}
	struct PNode *head = NULL;
	struct PNode *current = NULL;
	char *uid = get_user_uid();
	
	/* creates a linkedlist to store procs */
	while ((entry = readdir(dir)) != NULL) {
		char *dir_name = entry->d_name;
		if (is_pid(dir_name) && entry->d_type == DT_DIR
			&& (!user || is_user_proc(uid, dir_name))) {
			if (!head) {
				head = malloc(sizeof(struct PNode));
				head->pid = dir_name;
				current = head;
			} else {
				current->next = malloc(sizeof(struct PNode));
				current = current->next;
				current->pid = dir_name;
			}
		}
	}
	closedir(dir);
	return head;
}
