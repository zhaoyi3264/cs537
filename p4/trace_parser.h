#ifndef TRACE_PARSER_H
#define TRACE_PARSER_H

#include "process_table.h"

int is_number(char * str);

ProcT *parse_trace(char *fname);

#endif
