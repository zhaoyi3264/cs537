#include <ctype.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "process_control.h"

int get_argc(char *cmd) {
	char *str = malloc(sizeof(char) * strlen(cmd));
	sprintf(str, "%s", cmd);
	strtok(str, " ");
	int count = 1;
	while (strtok(NULL, " ")) {
		count++;
	}
	free(str);
	return count;
}

char *remove_space(char *str) {
	if (str == NULL) {
		return NULL;
	}
	char *temp = malloc(sizeof(char) * strlen(str));
	int j = 0;
	for (int i = 0; str[i]; i++) {
		if (isspace(str[i]) == 0) {
			temp[j++] = str[i];
		}
	}
	temp[j] = '\0';
	return temp;
}

void execute_command(char *cmd) {
	// replace tabs with spaces
	char *ws = NULL;
	while ((ws = index(cmd, '\t'))) {
		*ws = ' ';
	}
	// handle I/O redirection
	char *command = strtok(cmd, ">");
	char *out = strtok(NULL, ">");			
	char *in = NULL;
	if (index(command, '<')) {
		strtok(command, "<");
		in = strtok(NULL, "<");
	} else if (out && index(out, '<')) {
		strtok(out, "<");
		in = strtok(NULL, "<");
	}
	out = remove_space(out);
	in = remove_space(in);
	int new_in = 0;
	int new_out = 0;
	if (in) {
		new_in = open(in, O_RDONLY);
		dup2(new_in, STDIN_FILENO);
		close(new_in);
		free(in);
	}
	if (out) {
		new_out = open(out, O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR|S_IWUSR);
		dup2(new_out, STDOUT_FILENO);
		close(new_out);
		free(out);
	}
	// split command line
	int argc = get_argc(command);
	int size = 0;
	if (argc > 0) {
		size = argc + 1;
	} else {
		size = 3;
	}
	char *argv[size];
	argv[size - 1] = NULL;
	
	char *token = strtok(command, " ");
	argv[0] = malloc(sizeof(char) * strlen(token));
	sprintf(argv[0], "%s", token);
	for (int i = 1; i < argc; i++) {
		token = strtok(NULL, " ");
		argv[i] = malloc(sizeof(char) * strlen(token));
		sprintf(argv[i], "%s", token);
	}
	if (execvp(argv[0], argv)) {
		fprintf(stderr, "error: cannot execute command %s\n",
			command);
		exit(1);
	}
	exit(1);
}

void create_process(SpecNode *spec_node) {
	Node *current = spec_node->commands;
	while (current) {
		pid_t pid = fork();
		if (pid == 0) {
			execute_command(current->data);
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
