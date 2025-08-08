#include "my_grep.h"

int main(int argc, char *argv[]) {
  if (argc == 1) {
    print_error();
  } else {
    grep_flags select = {0};
    char templates[BUFFER] = {0};
    int count_flags = 0;
    int result = 0;
    result = get_flags(argc, argv, &select, &count_flags, templates);
    if (result == 1) {
      if (count_flags == 0){
        print_error();
      } else {
        print_msg();
      }
    } else if ((count_flags > 0) && (argc < 4) && (!select.e || !select.e)) print_msg();
    else {
        get_patterns(templates, argv, select);
        open_files(argc, argv, select, templates);
    }
  }
  return 0;
}

void print_error(){
  fprintf(stderr, "Usage: grep [OPTION]... PATTERNS [FILE]...\nTry 'grep --help' for more information.\n");
}

void print_msg(){
  fprintf(stderr, "grep: invalid max count\n");
}

int get_flags(int argc, char *argv[], grep_flags *select, int *count_flags,
              char *templates) {
  int iserror = 0, isflag = 0, count_e = 0;
  while ((isflag = getopt_long(argc, argv, "e:ivclnhsf:o?", NULL, NULL)) != -1) {
    switch (isflag) {
      case 'e':
        select->e = 1;
        add_pattern(optarg, &count_e, templates, select);
        break;
      case 'i':
        select->i = 1;
        break;
      case 'v':
        select->v = 1;
        break;
      case 'c':
        select->c = 1;
        break;
      case 'l':
        select->l = 1;
        break;
      case 'n':
        select->n = 1;
        break;
      case 'h':
        select->h = 1;
        break;
      case 's':
        select->s = 1;
        break;
      case 'f':
        select->f = 1;
        add_files(optarg, &count_e, templates, select);
        break;
      case 'o':
        select->o = 1;
        break;
      case '?':
        iserror = 1;
        break;
    }
    if (iserror == 0) *count_flags += 1;
  }
  if (select->empty_line) select->o = 0;
  if (select->c || select->l) select->n = 0;
  if (select->v) select->o = 0;

  return iserror;
}

void get_patterns(char *templates, char *argv[], grep_flags select) {
  if (!select.e && !select.f) {
    if (!argv[optind]) argv[optind] = ".";
    strcat(templates, argv[optind]);
    optind += 1;
  }
}

void open_files(int argc, char *argv[], grep_flags select, char *templates) {
  FILE *file = NULL;
  int count = argc - optind;
  if (count < 0) print_error();
  else{
    for (; optind < argc; optind++) {
      if ((file = fopen(argv[optind], "r")) != NULL) {
        if (!select.o && !select.v)
          comp_reg(argv, templates, file, &select, count);
        fclose(file);
      } else {
        if (select.s == 0) {
          fprintf(stderr, "grep: %s: No such file or directory\n", argv[optind]);
        }
      }
    }
  }
}

void comp_reg(char *argv[], char *templates, FILE *file, grep_flags *select,
              int count_files) {
  regex_t reg;
  regmatch_t pmatch[2] = {0};
  size_t nmatch = 1;
  int err, num_line = 1, lines_count = 0, is_i_select = 0;
  char string[256];
  if (select->i) is_i_select = REG_ICASE;
    err = regcomp(&reg, templates, REG_EXTENDED | is_i_select);
  if (err != 0) {
    char err_msg[50];
    regerror(err, &reg, err_msg, 50);
    fprintf(stderr, "Error regular expression '%s': %s\n", templates, err_msg);
    exit(3);
  }
  while (!feof(file)) {
    if (fgets(string, 256, file)) {
      err = regexec(&reg, string, nmatch, pmatch, 0);
      if (select->v) err = err ? 0 : 1;
      if (!err) {
        if (!select->c && !select->l) {
          int count_line = 1;
          if (count_files > 1 && !select->h) printf("%s:", argv[optind]);
          if (select->n) printf("%d:", num_line);
          if (select->o && select->f){
            fprintf(stderr, "grep: %s: No such file or directory\n", argv[optind]);
          }
          if (select->o && !select->v) {
            count_line = 0;
            char *ptr = string;
            while (!err) {
              if (pmatch[0].rm_eo == pmatch[0].rm_so) break;
              printf("%.*s\n", (int)(pmatch[0].rm_eo - pmatch[0].rm_so),
                     ptr + pmatch[0].rm_so);
              ptr += pmatch[0].rm_eo;
              err = regexec(&reg, ptr, nmatch, pmatch, REG_NOTBOL);
            }
          }
          if (!select->o || select->v) printf("%s", string);
          if (string[strlen(string) - 1] != '\n' && count_line) printf("\n");
        }
        lines_count++;
      }
      num_line++;
    }
  }
  if (select->c) {
    if (count_files > 1 && !select->h && !select->l) printf("%s:", argv[optind]);
    if (select->l && lines_count) lines_count--;
    else if (1 && !select->l) {
      printf("%d\n", lines_count);
    }
  }
  if (select->l && lines_count) {
    printf("%s\n", argv[optind]);
  }
  regfree(&reg);
}

void add_pattern(char *optarg, int *count_e, char *templates,
                 grep_flags *select) {
  if (*count_e) {
    char *ch = "|";
    strcat(templates, ch);
  }
  if (!*optarg) {
    optarg = ".";
    select->empty_line = 1;
  }
  strcat(templates, optarg);
  *count_e += 1;
}

void add_files(char *optarg, int *e_count, char *templates,
               grep_flags *select) {
  FILE *file = NULL;
  int empty_count = select->empty_line;
  if ((file = fopen(optarg, "r")) == NULL) {
    if (select->s == 0) {
      fprintf(stderr, "grep: %s: No such file or directory\n", optarg);
      exit(2);
    }
  } else {
    while (!feof(file)) {
      if (*e_count) {
        char *ch = "|";
        strcat(templates, ch);
      }
      size_t cur_len = strlen(templates);
      size_t new_len;
      fgets(templates + cur_len, 256, file);
      char *nch = strrchr(templates, 10);
      if (nch) *nch = '\0';
        new_len = strlen(templates);
      if (new_len == cur_len) {
        select->empty_line++;
        char *all = ".";
        strcat(templates, all);
      }
      *e_count += 1;
    }
    fclose(file);
  }
  if (templates[strlen(templates) - 1] == '.') {
    if ((select->empty_line - empty_count) == 1)
      select->empty_line = empty_count;
    templates[strlen(templates) - 1] = '\0';
  }
}