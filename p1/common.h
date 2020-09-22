/*
 * Authors: 
 * - Zhaoyi Zhang, netid: zzhang825
 * - Richard Li, netid: tli354
 */

#ifndef COMMON_H
#define COMMOM_H

struct Stat {
	int pid;
	char state;
	long unsigned int utime;
	long unsigned int stime;
};

struct PNode {
	char *pid;
	struct PNode *next;
};

#endif
