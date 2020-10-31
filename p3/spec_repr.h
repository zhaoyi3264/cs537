#ifndef SPEC_REPR_H
#define SPEC_REPR_H

// TODO: record line_num
typedef struct Node {
	char *data;
	struct Node *next;
} Node;

typedef struct SpecNode {
	char *target;
	struct SpecNode *next;
	Node *dependencies;
	Node *dependencies_tail;
	Node *commands;
	Node *commands_tail;
} SpecNode;

SpecNode *create_spec_node(char *target);

void add_dependency(SpecNode *spec_node, char *dependency);

void add_command(SpecNode *spec_node, char *command);

void print_dependency(SpecNode *spec_node);

void print_command(SpecNode *spec_node);

#endif
