#include "my_cat.h"

int main(int argc, char *argv[]) {
  if (argc >= 1) {
    cat_flags select = {0};
    int count = 0, count_flags = 0, count_files = 0, option_index = 0;

    static struct option long_options[] = {
        {"number-nonblank", no_argument, NULL, 'b'},
        {"number", no_argument, NULL, 'n'},
        {"squeeze-blank", no_argument, NULL, 's'},
        {0, 0, 0, 0},
    };
    int isflag;
    while ((isflag = getopt_long(argc, argv, "bveEnstT", long_options,
                                 &option_index)) != -1) {
      int result = 0;
      result = get_flags(&isflag, &select, &count_flags);
      if (result == 1) {
        exit(1);
      }
    }
    count_file(&count_files, argv, argc);
    int res;
    if ((count_flags > 0) && (count_files > 0)) {
      for (int i = 1; i < argc; i++) {
        reader(&select, argv[i]);
      }
    } else if ((count_flags == 0) && (count_files > 0)) {
      for (int i = 1; i < argc; i++) {
        res = print_text(argv[i]);
        if (res == -1) {
          fprintf(stderr, "cat: %s: No such file or directory \n", argv[i]);
          continue;
        }
        count++;
      }
    } else if ((argc > 1) && (count_files == 0) && (count_flags == 0)) {
      for (int i = 1; i < argc; i++) {
        fprintf(stderr, "cat: %s: No such file or directory\n", argv[i]);
      }
    }
  } else {
    fprintf(stderr, "Invalid error\n");
  }
  return 0;
}

void count_file(int *count_files, char *argv[], int argc) {
  for (int i = 1; i < argc; i++) {
    FILE *new_file = fopen(argv[i], "r");
    if ((new_file == NULL) && (strchr(argv[i], '-')) == NULL) {
      continue;
    } else if ((new_file != NULL) && (strchr(argv[i], '-')) == NULL) {
      *count_files += 1;
      fclose(new_file);
    }
  }
}

int print_text(char *files) {
  FILE *file = fopen(files, "r");
  if (file == NULL)
    return -1;
  else {
    int characture = fgetc(file);
    while (characture != EOF) {
      putc(characture, stdout);
      characture = fgetc(file);
    }
    fclose(file);
  }
  return 0;
}

int get_flags(int *isflag, cat_flags *select, int *count_flags) {
  int iserror = 0;
  switch (*isflag) {
    case 'b':
      select->b = 1;
      *count_flags += 1;
      break;
    case 'e':
      select->e = 1;
      select->v = 1;
      *count_flags += 2;
      break;
    case 'E':
      select->e = 1;
      *count_flags += 1;
      break;
    case 'n':
      select->n = 1;
      *count_flags += 1;
      break;
    case 's':
      select->s = 1;
      *count_flags += 1;
      break;
    case 't':
      select->t = 1;
      select->v = 1;
      *count_flags += 2;
      break;
    case 'T':
      select->t = 1;
      *count_flags += 1;
      break;
    case 'v':
      select->v = 1;
      *count_flags += 1;
      break;
    default:
      iserror = 1;
      break;
  }
  return iserror;
}

void reader(cat_flags *select, char *files) {
  int last_ch = '\n';
  FILE *file = NULL;
  int count_str = 0, count_empty_str = 0;
  if (((file = fopen(files, "r")) == NULL) && (strchr(files, '-')) == NULL) {
    fprintf(stderr, "cat: %s: No such file or directory\n", files);
  } else if ((strchr(files, '-')) == NULL) {
    while (1) {
      int cur_ch = fgetc(file);
      if (cur_ch == EOF) {
        break;
      }
      if (select->s && cur_ch == '\n' && last_ch == '\n') {
        count_empty_str += 1;
        if (count_empty_str > 1) {
          continue;
        }
      } else {
        count_empty_str = 0;
      }
      if (last_ch == '\n' && ((select->b && cur_ch != '\n'))) {
        printf("%6d\t", count_str += 1);
      }
      if (last_ch == '\n' && ((select->n) && (!select->b))) {
        printf("%6d\t", count_str += 1);
      }
      if ((select->t > 0 && cur_ch == '\t') ||
          (select->T > 0 && cur_ch == '\t')) {
        flag_tab(&cur_ch);
      }
      if ((select->e && cur_ch == '\n') || (select->E && cur_ch == '\n')) {
        printf("$");
      }
      if (select->v) {
        flag_invert_match(&cur_ch);
      }
      printf("%c", cur_ch);
      last_ch = cur_ch;
    }
    fclose(file);
  }
}

void flag_invert_match(int *cur_ch) {
  if ((0 <= *cur_ch && *cur_ch <= 8) || (11 <= *cur_ch && *cur_ch <= 31)) {
    putchar('^');
    *cur_ch += 64;
  } else if (*cur_ch == 127) {
    putchar('^');
    *cur_ch -= 64;
  }
}

void flag_tab(int *cur_ch) {
  putchar('^');
  *cur_ch = 'I';
}