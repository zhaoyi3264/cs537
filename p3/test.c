#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {
	FILE *fp = fopen("test", "r");
	int c;
	int index = 0;
	do {
		char *line = malloc(sizeof(char) * 4096);
		do {
			c = fgetc(fp);
			if (c == '\0') {
				fprintf(stderr, "Null character encountered at line No %d. Exiting the program", 1);
			}
			line[index++] = (char) c;
		} while (c != '\n' && c != EOF && index < 4096);

		if (index >= 4096) {
			fprintf(stderr, "THIS LINE EXHAUSTED THE BUFFER SIZE %d. Terminating the program\n", 4096);
			free(line);
			exit(1);
		} else {
			line[index - 1] = '\0';
		}
		printf("%s\n", line);
		index = 0;
		free(line);
	} while (c != EOF);
}
