/*
 * Authors: 
 * - Zhaoyi Zhang, netid: zzhang825
 * - Richard Li, netid: tli354
 */

#ifndef OPTIONS_PROCESSING_H
#define OPTIONS_PROCESSING_H

int toggle_flag();

struct PNode *parse_cmdline_options(int argc, char *argv[],
	int *p, int *state, int *utime, int *stime, int *vm, int *cmd);

#endif
