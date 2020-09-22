/*
 * Authors: 
 * - Zhaoyi Zhang, netid: zzhang825
 * - Richard Li, netid: tli354
 */

#ifndef STAT_FILE_PARSER_H
#define STAT_FILE_PARSER_H

void parse_stat(char *file, struct Stat *stat);

int parse_statm(char *file);

void get_cmd(char *file, char *cmd);

FILE *open_file(char *file);

void close_file(FILE *fp);

#endif
