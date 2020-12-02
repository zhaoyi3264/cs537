/*
 * Trace file parsing module
 * 
 * Authors: 
 * - Zhang, Zhaoyi, zhaoyi, zzhang825
 * - Li, Richard, richardl, tli354
 */

#ifndef TRACE_PARSER_H
#define TRACE_PARSER_H

#include "process_table.h"

int is_number(char * str);

FILE *open_trace(char *fname);

ProcT *parse_trace(char *fname);

#endif
