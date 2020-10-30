#include "spec_graph.h"

int is_blank(char *line);

int is_target (char *line);

int is_cmd (char *line);

SpecGraph *parse_makefile (char *fname);
