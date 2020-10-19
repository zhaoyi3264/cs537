#include <stdio.h>

void parse_makefile (char *fname) {
	FILE *fp = fopen(fname, "r");
	char * line = NULL;
    size_t len = 0;
    int size;
    int buf_limit = 4096;
    while ((size = getline(&line, &len, fp)) != -1) {
		if (size > buf_limit) {
			printf("Retrieved line of length %d:\n", size);
			printf("%s", line);
		}
    }
  
	fclose(fp);
}
