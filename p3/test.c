#include <stdlib.h>
#include <stdio.h>

typedef struct Node {
	char *data;
	struct Node *next;
} Node;

int main() {
	Node *head = malloc(sizeof(Node));
	head->data = "target";
	Node *current = head;
	for (int i = 0; i < 10; i++) {
		char *buf = malloc(sizeof(char) * 10);
		current->next = malloc(sizeof(Node));
		current = current->next;
		sprintf(buf, "%d", i);
		current->data = buf;
	}
	current = head;
	while (current) {
		printf("%s\n", current->data);
		current = current->next;
	}
}
