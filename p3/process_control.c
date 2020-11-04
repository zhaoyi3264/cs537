/*
 * Process control module
 * 
 * Authors: 
 * - Zhang, Zhaoyi, zhaoyi, zzhang825
 * - Li, Richard, richardl, tli354
 */
 
#include <ctype.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "process_control.h"

/*
 * Get number of arguments of a command
 * 
 * cmd: command
 * 
 * return: number of arguments
 */
int get_argc(char *cmd) {
	char *str = malloc(sizeof(char) * strlen(cmd));
	if (str == NULL || (sprintf(str, "%s", cmd) < 0)) {
		exit(1);
	}
	strtok(str, " ");
	int count = 1;
	while (strtok(NULL, " ")) {
		count++;
	}
	free(str);
	return count;
}

/*
 * Remove spaces in the string
 * 
 * str: string
 * 
 * return: the string without spaces
 */
char *remove_space(char *str) {
	if (str == NULL) {
		return NULL;
	}
	char *temp = malloc(sizeof(char) * strlen(str));
	if (temp == NULL) {
		exit(1);
	}
	int j = 0;
	for (int i = 0; str[i]; i++) {
		if (isspace(str[i]) == 0) {
			temp[j++] = str[i];
		}
	}
	temp[j] = '\0';
	return temp;
}

/*
 * Execute a command and handle I/O redirection
 * 
 * cmd: command
 */
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
		if ((new_in = open(in, O_RDONLY)) == -1) {
			exit(1);
		}
		dup2(new_in, STDIN_FILENO);
		if (close(new_in) == -1) {
			exit(1);
		}
	}
	if (out) {
		if ((new_out = open(out, O_CREAT|O_WRONLY|O_TRUNC,
			S_IRUSR|S_IWUSR)) == -1) {
			exit(1);
		}
		dup2(new_out, STDOUT_FILENO);
		if (close(new_out) == -1) {
			exit(1);
		}
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
	if (argv[0] == NULL || (sprintf(argv[0], "%s", token) < 0)) {
		exit(1);
	}
	for (int i = 1; i < argc; i++) {
		token = strtok(NULL, " ");
		argv[i] = malloc(sizeof(char) * strlen(token));
		if (argv[i] == NULL || (sprintf(argv[i], "%s", token) < 0)) {
			exit(1);
		}
	}
	if (execvp(argv[0], argv)) {
		fprintf(stderr, "error: cannot execute command %s\n",
			command);
		exit(1);
	}
	exit(1);
}

/*
 * Create a process to execute commands in the specification node
 * 
 * cmd: command to start the process
 */
void create_process(char *cmd) {
	pid_t pid = fork();
	if (pid == 0) {
		char *copy = malloc(sizeof(char) * strlen(cmd) + 1);
		if (copy == NULL || (sprintf(copy, "%s", cmd) < 0)) {
			exit(1);
		}
		execute_command(copy);
		free(copy);
	} else if (pid < 0) {
		fprintf(stderr, "error: cannot fork child process\n");
		exit(1);
	} else {
		int stat_loc;
		if ((waitpid(pid, &stat_loc, 0) == -1) || stat_loc) {
			exit(1);
		}
	}
}
