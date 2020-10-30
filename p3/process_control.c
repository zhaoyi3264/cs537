#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "process_control.h"
#include "spec_repr.h"

void createProcess(SpecNode *spec_node) {
	
    pid_t pid;
    int buf_limit = 4096;
	char* argv[buf_limit];
    int status;

    char command_line = spec_node->commands->data;

    for(int i = 0; i <buf_limit; i++) {
        argv[i] = NULL;
    }

    pid = fork();

    if(pid < 0){
        fprintf(stderr, "Error: Fork failed\n");
        exit(-1);
    }

    else if(pid ==0){
        int i = 0;
        char *split = strtok(command_line, " ");

        while (split) {
            argv[i] = split;
            i++;
            split = strtok(NULL, " ");
        }

        if (argv[0] == NULL) {
            fprintf(stderr, "Error: Command not found\n");
            exit(-1);
        }

        if(execvp(argv[0], argv)){
            fprintf(stderr, "Error: execvp failed\n");
            exit(-1);
        }
    }
    else {
        while(wait(&status) != pid){
            // waiting
        }
    } 
}
