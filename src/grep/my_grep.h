#ifndef MY_GREP_H
#define MY_GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER 2048

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int o;
  int f;
  int s;
  int empty_line;
} grep_flags;

int get_flags(int argc, char *argv[], grep_flags *select, int *count_flags,
              char *templates);
void get_patterns(char *templates, char *argv[], grep_flags select);
void open_files(int argc, char *argv[], grep_flags select, char *templates);
void comp_reg(char *argv[], char *templates, FILE *file, grep_flags *select,
              int count);
void add_pattern(char *optarg, int *count_e, char *templates,
                 grep_flags *select);
void add_files(char *optarg, int *e_count, char *templates, grep_flags *select);
void flag_c(int count_files, int lines_count);
void print_error();
void print_msg();

#endif
