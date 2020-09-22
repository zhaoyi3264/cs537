/*
 * Authors: 
 * - Zhaoyi Zhang, netid: zzhang825
 * - Richard Li, netid: tli354
 */

#ifndef PROCESS_LIST_H
#define PROCESS_LIST_H

char *get_proc_uid(char *file);

char *get_user_uid();

int is_pid(char *dir_name);

int is_user_proc(char *uid, char *pid);

struct PNode *get_proc(int user);

#endif
