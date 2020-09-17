int toggle_flag();

struct PNode *parse_cmdline_options(int argc, char *argv[],
	int *p, int *state, int *utime, int *stime, int *vm, int *cmd);
