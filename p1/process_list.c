#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include "process_list.h"

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

char *get_user_uid() {
	return get_proc_uid("/proc/self/status");
}

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

int is_user_proc(char *uid, char *pid) {
	char file[50] = "/proc/";
	char *proc_uid = get_proc_uid(strcat(strcat(file, pid), "/status"));
	return !strcmp(uid, proc_uid);
}

struct PNode *get_user_proc() {
	DIR *dir;
	struct dirent *entry;
	if ((dir = opendir("/proc")) == NULL) {
		fprintf(stderr, "Error reading /proc directory");
		exit(1);
	}
	struct PNode *head = NULL;
	struct PNode *current = NULL;
	char *uid = get_user_uid();
	while ((entry = readdir(dir)) != NULL) {
		char *dir_name = entry->d_name;
		if (is_pid(dir_name) && entry->d_type == DT_DIR
			&& is_user_proc(uid, dir_name)) {
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
