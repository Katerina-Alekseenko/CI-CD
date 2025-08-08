#ifndef MY_CAT_H
#define MY_CAT_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
  int b;
  int e;
  int s;
  int n;
  int v;
  int t;
  int E;
  int T;
} cat_flags;

int get_flags(int *isflag, cat_flags *select, int *count_flags);
int print_text(char *files);
void count_file(int *count_files, char *argv[], int argc);
void reader(cat_flags *select, char *files);
void flag_invert_match(int *cur_ch);
void flag_tab(int *cur_ch);

#endif
