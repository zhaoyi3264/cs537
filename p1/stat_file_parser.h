struct Stat {
	int pid;
	char state;
	int utime;
	int stime;
};

void parse_stat(char *file, struct Stat *stat);

int parse_statm(char *file);

void check_file_name(char *file, char *target);
