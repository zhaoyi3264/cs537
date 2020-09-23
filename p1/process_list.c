/*
 * Provides methods for getting current user's processes
 * 
 * Authors: 
 * - Zhaoyi Zhang, netid: zzhang825
 * - Richard Li, netid: tli354
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include "common.h"
#include "process_list.h"

/*
 * Gets the uid of a process
 * 
 * file: the path of the status file of a process
 * 
 * return: the uid of the process
 */
char *get_proc_uid(char *file) {
	FILE *fp = fopen(file, "r");
	if (fp == NULL) {
		exit(1);
	}
	char line[256];
	char temp[4];
	char *uid = malloc(10 * sizeof(char));
	if (uid == NULL) {
		exit(1);
	}
	while (fgets(line, sizeof(line), fp)) {
		// find the line that starts with "Uid:"
		if (!strncmp(line, "Uid:", 4)) {
			if (sscanf(line, "%s\t%s", temp, uid) < 0) {
				exit(1);
			}
		}
	}
	if (fclose(fp) != 0) {
		exit(1);
	}
	return uid;
}

/*
 * Gets uid of the current user
 * 
 * return: the uid of current user
 */
char *get_user_uid() {
	return get_proc_uid("/proc/self/status");
}

/*
 * Checks if the name of a directory under /proc is a valid pid
 * 
 * dir_name: the name of a directory under /proc
 * 
 * return: 1 if the name if valid
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
 * Checks if a process belongs to the current user
 * 
 * uid: the uid of the current user
 * pid: the pid of a process to check
 * 
 * return: 1 if the process belongs to the current user
 */
int is_user_proc(char *uid, char *pid) {
	char file[64];
	if (snprintf(file, 64, "/proc/%s/status", pid) < 0) {
		exit(1);
	}
	char *proc_uid = get_proc_uid(file);
	return !strcmp(uid, proc_uid);
}

/*
 * Gets a list of processes found under /proc
 * 
 * user: if to only get the processes that belong to the current user
 * 
 * return: a linked list of processes
 */
struct PNode *get_proc(int user) {
	DIR *dir;
	struct dirent *entry;
	if ((dir = opendir("/proc")) == NULL) {
		exit(1);
	}
	struct PNode *head = NULL;
	struct PNode *current = NULL;
	char *uid = get_user_uid();
	// iterate through the directory and create the linked list
	while ((entry = readdir(dir)) != NULL) {
		char *dir_name = entry->d_name;
		// check if the directory name is a pid
		if (entry->d_type == DT_DIR && is_pid(dir_name)
			&& (!user || is_user_proc(uid, dir_name))) {
			if (!head) {
				head = malloc(sizeof(struct PNode));
				if (head == NULL) {
					exit(1);
				}
				current = head;
			} else {
				current->next = malloc(sizeof(struct PNode));
				current = current->next;
				if (current == NULL) {
					exit(1);
				}
			}
			current->pid = dir_name;
		}
	}
	closedir(dir);
	return head;
}
