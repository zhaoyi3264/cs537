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
