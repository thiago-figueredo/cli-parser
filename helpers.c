#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>
#include "helpers.h"

const char* get_filename(const char *path) {
  assert(path != NULL);

  char* filename = strrchr(path, '/');

  if (filename) {
    return filename + 1;
  }

  return path;
}

char* trim_str(char *str) {
  if (!str) return str;

  int start = 0, end = strlen(str) - 1;
  bool has_space = false;

  while (isspace(str[start])) {
    has_space = true;
    start++;
  }

  while (end >= start && isspace(str[end])) {
    has_space = true;
    end--;
  }

  if (!has_space) return str;

  int j = 0;

  for (int i = start; i <= end; i++) {
    str[j++] = str[i];
  }

  str[j] = '\0';

  return str;
}