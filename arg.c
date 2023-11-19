#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "arg.h"

Arg* arg_empty() {
  Arg* arg = malloc(sizeof(*arg));
  assert(arg != NULL);
  return arg;
}

Arg* arg_new(char* short_name, char* long_name, char* description,  void* value, ArgKind kind, bool required) {
  Arg* arg = malloc(sizeof(*arg));
  assert(arg != NULL);

  *arg = (Arg) {
    .required = required,
    .value = value,
    .kind = kind,
    .next = NULL
  };

  arg->short_name = malloc((strlen(short_name) + 1) * sizeof(char)),
  assert(arg->short_name != NULL);
  assert(strcpy(arg->short_name, short_name) != NULL);

  arg->long_name = malloc((strlen(long_name) + 1) * sizeof(char)),
  assert(arg->long_name != NULL);
  assert(strcpy(arg->long_name, long_name) != NULL);

  arg->description = malloc((strlen(description) + 1) * sizeof(char)),
  assert(arg->description != NULL);
  assert(strcpy(arg->description, description) != NULL);

  return arg;
}

Arg* arg_string(char* short_name, char* long_name, char* description, char* value) {
  char* value_ptr = value;

  if (value) {
    value_ptr = malloc((strlen(value) + 1) * sizeof(char));
    assert(value_ptr != NULL);

    strcpy(value_ptr, value);
  }

  return arg_new(short_name, long_name, description, (void*)value_ptr, ARG_STRING, false);
}

Arg* arg_string_required(char* short_name, char* long_name, char* description) {
  Arg* arg = arg_string(short_name, long_name, description, NULL);
  arg->required = true;

  return arg;
}

Arg* arg_int(char* short_name, char* long_name, char* description, int value) {
  int* value_ptr = malloc(sizeof(*value_ptr));
  assert(value_ptr != NULL);

  *value_ptr = value;

  return arg_new(short_name, long_name, description, (void*)value_ptr, ARG_INT, false);
}

Arg* arg_int_required(char* short_name, char* long_name, char* description) {
  return arg_new(short_name, long_name, description, NULL, ARG_INT, false);
}

Arg* arg_bool(char* short_name, char* long_name, char* description, bool value) {
  bool* value_ptr = malloc(sizeof(*value_ptr));
  assert(value_ptr != NULL);

  *value_ptr = value;
  return arg_new(short_name, long_name, description, (void*)value_ptr, ARG_BOOL, false);
}

Arg* arg_bool_required(char* short_name, char* long_name, char* description) {
  return arg_new(short_name, long_name, description, NULL, ARG_BOOL, false);
}

void arg_print_value(Arg* arg) {
  assert(arg != NULL);

  if (arg->kind == ARG_STRING) {
    printf("%s", (char*)(arg->value));
  } else if (arg->kind == ARG_INT) {
    printf("%d", *(int*)(arg->value));
  } else if (arg->kind == ARG_BOOL) {
    printf("%s", *(bool*)(arg->value) ? "true" : "false");
  } else {
    fprintf(stderr, "ERROR: Unknown arg kind");
    exit(EXIT_FAILURE);
  }
}

void free_arg(Arg* arg) {
  Arg* current = arg;
  Arg* temp = NULL;

  while (current) {
    temp = current;
    current = current->next;

    free((void*)temp->value);
    free(temp->short_name);
    free(temp->long_name);
    free(temp->description);
    free(temp);
  }
}