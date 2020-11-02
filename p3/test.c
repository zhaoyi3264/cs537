#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
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

int main() {
	struct stat *statbuf = malloc(sizeof(struct stat));
	int fd = open("queue.o", O_RDONLY);
	fstat(fd, statbuf);
	printf("%ld\n", (&(statbuf->st_mtim))->tv_sec);
}
