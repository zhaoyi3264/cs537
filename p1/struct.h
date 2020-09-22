/*
 * Authors: 
 * Zhaoyi Zhang, netid: zzhang825
 * Richard Li, netid: tli354
 */

struct Stat {
	int pid;
	char state;
	int utime;
	int stime;
};

struct PNode {
	char *pid;
	struct PNode *next;
};
