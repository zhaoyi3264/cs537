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
		char *argv[] = {"-l", "test", NULL};
		execvp("wc", argv);
		//~ exit(1);
	} else if (pid == -1) {
		printf("error\n");
	} else {
		waitpid(pid, &stat_loc, 0);
		printf("parent with child exit %d\n", stat_loc);
	}
}

int is_outdated(struct stat *target, struct stat *dependency) {
	
}

int main() {
	struct stat *statbuf = malloc(sizeof(struct stat));
	stat("test", statbuf);
	printf("%ld\n", statbuf->st_dev);
	struct timespec *st_mtim = &(statbuf->st_mtim);
	printf("%ld\n", st_mtim->tv_sec);
}
