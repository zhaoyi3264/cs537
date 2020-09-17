void parse_stat(char *file, struct Stat *stat);

int parse_statm(char *file);

void get_cmd(char *file, char *cmd);

FILE *open_file(char *file);

void close_file(char *file, FILE *fp);

void check_file_name(char *file, char *target);
