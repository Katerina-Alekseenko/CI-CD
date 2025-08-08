#include "my_grep.h"

int main(int argc, char **argv) {
  int flag_err = 1;
  if (argc > 2) {
    grep_flags select = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    char pattern[BUFFER] = {0};
    int result = 0;
    char *optstring = "violnce:f:sh?";
    while ((result = getopt_long(argc, argv, optstring, 0, NULL)) != -1) {
      flag_err = get_flags(&select, result, pattern);
    }
    if ((select.e || select.f) & (argc < 4)) {
      flag_err = 2;
    }
    if (flag_err != 2) {
      variant((const char **)argv, pattern, &select);
    }
  }
  return flag_err;
}

int get_flags(grep_flags *select, int result, char *pattern) {
  int flag_err = 0;
  switch (result) {
    case 'v':
      select->v = 1;
      break;
    case 'i':
      select->i = 1;
      break;
    case 'o':
      select->o = 1;
      break;
    case 'l':
      select->l = 1;
      break;
    case 'n':
      select->n = 1;
      break;
    case 'c':
      select->c = 1;
      break;
    case 'e':
      select->e = 1;
      get_pattern(pattern, optarg);
      break;
    case 'f':
      select->f = 1;
      flag_err = flag_f_handler(pattern);
      break;
    case 's':
      select->s = 1;
      break;
    case 'h':
      select->h = 1;
      break;
    case '?':
      flag_err = 2;
  }
  if (select->v) select->o = 0;
  return flag_err;
}

int variant(const char **argv, const char *pattern, grep_flags const *select) {
  int flag_err = 1;
  int count_files = 0;
  int flag_no_pattern = 0;
  if (select->e || select->f) {
    count_files = file_count(argv, flag_no_pattern);
  } else {
    flag_no_pattern = 1;
    pattern = argv[optind];
    count_files = file_count(argv, flag_no_pattern);
  }
  flag_err = comp_reg(argv, pattern, count_files, flag_no_pattern, select);
  return flag_err;
}

void get_pattern(char *pattern, const char *string) {
  if (0 == *string) {
    strcpy(pattern, ".*\0");
  }
  if (0 == pattern[0]) {
    strcpy(pattern, string);
  } else if (strcmp(pattern, ".*") != 0) {
    strcat(pattern, "|");
    strcat(pattern, string);
  }
}

int file_count(const char **argv, int flag_no_pattern) {
  int count_files = 0;
  int count = optind;
  if (flag_no_pattern) {
    count += 1;
  }
  for (int i = count; NULL != argv[i]; i++) {
    if (argv[i][0] != '-') {
      count_files += 1;
    }
  }
  return count_files;
}

int comp_reg(const char **argv, const char *pattern, int count_files,
             int flag_no_pattern, grep_flags const *select) {
  int flag_err = 1;
  for (int ind = 0; ind < count_files; ind++) {
    FILE *file_ptr;
    int file_arg = optind + ind + flag_no_pattern;
    const char *file_name = argv[file_arg];
    if ((file_ptr = fopen(file_name, "r")) == NULL) {
      if (!select->s) {
        fprintf(stderr, "my_grep: %s: %s\n", file_name, strerror(2));
      }
      flag_err = 2;
    } else {
      char is_l_select = 0;
      regex_t reg_exp;
      unsigned int num_strings = 0;
      int reg = select->i ? regcomp(&reg_exp, pattern, REG_ICASE)
                          : regcomp(&reg_exp, pattern, REG_EXTENDED);
      if (reg != 0) {
        char reg_errbuf[LITTLE_SIZE] = {0};
        regerror(reg, &reg_exp, reg_errbuf, LITTLE_SIZE);
        fprintf(stderr, "Regexp compilation failed: '%s'\n", reg_errbuf);
        flag_err = 2;
      }
      if (flag_err != 2) {
        char buf_str[BUFFER] = {0};
        for (int num_str = 1; NULL != fgets(buf_str, BUFFER, file_ptr);
             ++num_str) {
          if ((!select->v && (regexec(&reg_exp, buf_str, 0, NULL, 0) == 0)) ||
              (select->v && (regexec(&reg_exp, buf_str, 0, NULL, 0) != 0))) {
            if (select->c) {
              select->l ? num_strings = 1 : ++num_strings;
            }
            if (select->l) {
              is_l_select = 1;
            } else {
              flag_err = opt_handler(file_name, count_files, num_str, buf_str,
                                     pattern, select);
            }
          }
        }
      }
      if (select->c) {
        flag_err = flag_c_handler(select, count_files, file_name, num_strings);
      }
      if (is_l_select == 1) {
        printf("%s\n", file_name);
        flag_err = 0;
      }
      regfree(&reg_exp);
      fclose(file_ptr);
    }
  }
  return flag_err;
}

int opt_handler(const char *file_name, int count_files, int num_str,
                char *buf_str, const char *pattern, grep_flags const *select) {
  int flag_err = 1;
  if (!select->c) {
    if (count_files > 1 && !select->h) {
      printf("%s:", file_name);
    }
    if (select->n) {
      printf("%d:", num_str);
    }
    if (select->o && !select->v) {
      flag_err = flag_o_handler(select, buf_str, pattern);
    } else {
      fputs(buf_str, stdout);
      flag_err = 0;
    }
    if (!select->o) {
      int n = strlen(buf_str);
      if (buf_str[n] == '\0' && buf_str[n - 1] != '\n') {
        putchar('\n');
      }
    }
  }
  return flag_err;
}

int flag_c_handler(grep_flags const *select, int count_files,
                   const char *file_name, unsigned int num_strings) {
  int flag_err = 1;
  if ((count_files > 1) && !select->h) {
    printf("%s:%u\n", file_name, num_strings);
    flag_err = 0;
  } else {
    printf("%u\n", num_strings);
    flag_err = 0;
  }
  return flag_err;
}

int flag_o_handler(grep_flags const *select, char *buf_str,
                   const char *pattern) {
  int flag_err = 1;
  regex_t reg_exp;
  int reg = select->i ? regcomp(&reg_exp, pattern, REG_ICASE)
                      : regcomp(&reg_exp, pattern, REG_EXTENDED);
  if (0 != reg) {
    char reg_buf[LITTLE_SIZE] = {0};
    regerror(reg, &reg_exp, reg_buf, LITTLE_SIZE);
    fprintf(stderr, "Regexp compilation failed: '%s'\n", reg_buf);
    flag_err = 2;
  }
  if (0 == reg && !select->v) {
    regmatch_t pmatch[BUFFER];
    char *s = buf_str;
    for (int i = 0; buf_str[i] != '\0'; ++i) {
      if (0 != regexec(&reg_exp, s, 1, pmatch, 0)) {
        break;
      }
      printf("%.*s\n", (int)(pmatch->rm_eo - pmatch->rm_so), s + pmatch->rm_so);
      s += pmatch->rm_eo;
      flag_err = 0;
    }
  }
  regfree(&reg_exp);
  return flag_err;
}

int flag_f_handler(char *pattern) {
  int flag_err = 1;
  FILE *file_pattern_ptr;
  const char *file_name_pattern = optarg;
  if (NULL == (file_pattern_ptr = fopen(file_name_pattern, "r"))) {
    fprintf(stderr, "grep: %s: %s\n", file_name_pattern, strerror(2));
    flag_err = 2;
  } else {
    char buf_pattern[BUFFER] = {0};
    while (NULL != fgets(buf_pattern, BUFFER, file_pattern_ptr)) {
      if ('\n' == *buf_pattern) {
        strcpy(pattern, ".*\0");
      } else {
        buf_pattern[strlen(buf_pattern) - 1] = '\0';
        get_pattern(pattern, buf_pattern);
      }
    }
    fclose(file_pattern_ptr);
  }
  return flag_err;
}
