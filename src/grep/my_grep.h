#ifndef SRC_GREP_MY_GREP_H_
#define SRC_GREP_MY_GREP_H_

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER 1024
#define LITTLE_SIZE 128

typedef struct {
  char v;
  char i;
  char o;
  char l;
  char n;
  char c;
  char e;
  char f;
  char s;
  char h;
} grep_flags;

void get_pattern(char *pattern, const char *src);
int get_flags(grep_flags *select, int result, char *pattern);
int variant(const char **argv, const char *pattern, grep_flags const *select);
int comp_reg(const char **argv, const char *pattern, int count_files,
             int flag_no_pattern_opt, grep_flags const *select);
int file_count(const char **argv, int flag_no_pattern_opt);
int opt_handler(const char *file_name, int count_files, int num_str,
                char *buf_str, const char *pattern, grep_flags const *select);
int flag_f_handler(char *pattern);
int flag_o_handler(grep_flags const *select, char *buffer, const char *pattern);
int flag_c_handler(grep_flags const *select, int count_files,
                   const char *file_name, unsigned int num_strings);

#endif
