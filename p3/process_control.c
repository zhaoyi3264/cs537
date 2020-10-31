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
	int count = 0;
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
		int stat_loc;
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
				size = 2;
			}
			char *argv[size];
			if (argc == 0) {
				argv[0] = "";
			}
			argv[size - 1] = NULL;
			
			char *file = strtok(cmd, " ");
			char *token;
			for (int i = 0; i < argc; i++) {
				token = strtok(NULL, " ");
				argv[i] = malloc(sizeof(char) * strlen(token));
				sprintf(argv[i], "%s", token);
			}
			//~ printf("argv: ");
			//~ for (int i = 0; i < argc; i++) {
				//~ printf("%s ", argv[i]);
			//~ }
			//~ printf("\n");
			if (execvp(file, argv)) {
				fprintf(stderr, "Error: execvp failed\n");
				exit(-1);
			}
		} else if (pid < 0) {
			fprintf(stderr, ": Fork failed\n");
			exit(1);
		} else {
			waitpid(pid, &stat_loc, 0);
			// TODO: check stat_loc
		}
		current = current->next;
	}
}
