struct PNode {
	char *pid;
	struct PNode *next;
};

char *get_proc_uid(char *file);

char *get_user_uid();

int is_pid(char *dir_name);

int is_user_proc(char *uid, char *pid);

struct PNode *get_proc(int user);
