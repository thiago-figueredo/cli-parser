#ifndef __ARG__H__
#define __ARG__H__

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

typedef enum {
  ARG_STRING,
  ARG_INT,
  ARG_BOOL,
} ArgKind;

typedef struct Arg {
  /** 
   * Represents both either the value or default value of the arg.
   * When parsing the cli, if value is not NULL, then we assume that the value is 
   * the default value. Otherwise, we assume that the value is the value passed 
  **/ 
  void* value;

  // We decided to allow short name be whatever the user wants.
  char* short_name;

  // Must be longer than short_name
  char* long_name;

  char* description;
  bool required;
  ArgKind kind;
  struct Arg* next;
} Arg;

Arg* arg_empty();
Arg* arg_string(char* short_name, char* long_name, char* description, char* value);
Arg* arg_string_required(char* short_name, char* long_name, char* description);
Arg* arg_int(char* short_name, char* long_name, char* description, int value);
Arg* arg_int_required(char* short_name, char* long_name, char* description);
Arg* arg_bool(char* short_name, char* long_name, char* description, bool value);
Arg* arg_bool_required(char* short_name, char* long_name, char* description);
void free_arg(Arg* arg);
void arg_print_value(Arg* arg);

#endif