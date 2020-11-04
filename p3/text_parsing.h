/*
 * Authors: 
 * - Zhang, Zhaoyi, zhaoyi, zzhang825
 * - Li, Richard, richardl, tli354
 */
 
#ifndef TEXT_PARSING_H
#define TEXT_PARSING_H

#include "spec_graph.h"

int is_blank(char *line);

int is_target (char *line);

int is_cmd (char *line);

SpecGraph *parse_makefile (char *fname);

#endif
