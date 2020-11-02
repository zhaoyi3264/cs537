#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "process_control.h"

int get_argc(char *cmd) {
	char *str = malloc(sizeof(char) * strlen(cmd));
	sprintf(str, "%s", cmd);
	char *token = strtok(str, " ");
	int count = 1;
	while ((token = strtok(NULL, " "))) {
		count++;
	}
	free(str);
	return count;
}

void create_process(SpecNode *spec_node) {
	Node *current = spec_node->commands;
	while (current) {
		pid_t pid;
		pid = fork();

		if (pid == 0) {
			char *cmd = current->data;
			// replace tabs with spaces
			char *ws = NULL;
			while ((ws = index(cmd, '\t'))) {
				*ws = ' ';
			}
			int argc = get_argc(cmd);
			//~ printf("argc: %d\n", argc);
			int size = 0;
			if (argc > 0) {
				size = argc + 1;
			} else {
				size = 3;
			}
			char *argv[size];
			argv[size - 1] = NULL;
			
			char *token = strtok(cmd, " ");
			argv[0] = malloc(sizeof(char) * strlen(token));
			sprintf(argv[0], "%s", token);
			for (int i = 1; i < argc; i++) {
				token = strtok(NULL, " ");
				argv[i] = malloc(sizeof(char) * strlen(token));
				sprintf(argv[i], "%s", token);
			}
			//~ printf("file: %s, argv: ", argv[0]);
			//~ for (int i = 0; i < argc; i++) {
				//~ printf("%s ", argv[i]);
			//~ }
			//~ printf("\n");
			if (execvp(argv[0], argv)) {
				fprintf(stderr, "error: cannot execute command %s\n",
					current->data);
				exit(1);
			}
		} else if (pid < 0) {
			fprintf(stderr, "error: cannot fork child process\n");
			exit(1);
		} else {
			int stat_loc;
			waitpid(pid, &stat_loc, 0);
			if (stat_loc) {
				exit(1);
			}
		}
		current = current->next;
	}
}
