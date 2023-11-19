#ifndef __CLI__H__
#define __CLI__H__

#include "list.h"
#include "arg.h"

typedef struct {
  const char* name;
  const char* description;
  int argc;
  char** argv;
  List* arg_list;
} Cli;

typedef enum {
  ENTRY_STRING,
  ENTRY_INT,
  ENTRY_BOOL,
} EntryKind;

typedef struct Entry {
  const char* key;
  void* value;
  EntryKind kind;
} Entry;

Cli cli_init(const char* name, const char* description, int argc, char** argv);
void cli_add_arg(Cli* cli, Arg* arg);
char* cli_get_string(Cli* cli, const char* key);
bool* cli_get_bool(Cli* cli, const char* key);
int* cli_get_int(Cli* cli, const char* key);
Arg* cli_find_arg(Cli* cli, Arg* arg);
void cli_free(Cli* cli);
void cli_dump_help(Cli* cli);
List* cli_parse(Cli* cli);

void print_entry(Entry* entry);

#endif