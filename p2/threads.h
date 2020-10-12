/*
 * Main driver module
 * 
 * Authors: 
 * - Zhaoyi Zhang, netid: zzhang825
 * - Richard Li, netid: tli354
 */
 
#ifndef THREADS_H
#define THREADS_H

void *reader(void *args);

void *munch1(void *args);

void *munch2(void *args);

void *writer(void *args);

#endif
