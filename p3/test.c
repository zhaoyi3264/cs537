#include <ctype.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void fork_test() {
	int stat_loc;
	pid_t pid = fork();
	if (pid == 0) {
		printf("child\n");
		char *file = "sleep";
		char *argv[] = {file, "10", NULL};
		if (execvp(file, argv)) {
			exit(1);
		}
	} else if (pid == -1) {
		printf("error\n");
	} else {
		waitpid(pid, &stat_loc, 0);
		printf("parent with child exit %d\n", stat_loc);
	}
}

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

char *remove_space(char *str) {
	if (str == NULL) {
		return NULL;
	}
	char *temp = malloc(sizeof(char) * strlen(str));
	for (int i = 0, j = 0; str[i]; i++) {
		if (isspace(str[i]) == 0) {
			temp[j++] = str[i];
		}
	}
	return temp;
}

int main() {
	pid_t pid = fork();
	if (pid == 0) {
		char *cmd = malloc(128);
		sprintf(cmd, "%s", "cat <makefile > else");
		// replace tabs with spaces
		char *ws = NULL;
		while ((ws = index(cmd, '\t'))) {
			*ws = ' ';
		}
		
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
		printf("cmd: *%s*\n", command);
		printf("out: *%s*\n", out);
		printf("in: *%s*\n", in);
		int new_in = 0;
		int new_out = 0;
		if (in) {
			new_in = open(in, O_RDONLY);
			dup2(new_in, STDIN_FILENO);
			close(new_in);
		}
		if (out) {
			new_out = open(out, O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR|S_IWUSR);
			dup2(new_out, STDOUT_FILENO);
			close(new_out);
		}
		printf("in: %d, out: %d\n", new_in, new_out);

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
}
