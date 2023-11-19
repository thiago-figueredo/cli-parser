#ifndef __CLI__H__
#define __CLI__H__

#include "arg.h"

typedef struct {
  const char* name;
  const char* description;
  int argc;
  char** argv;
  size_t args_count;
  Arg* args_head;
  Arg* args_tail;
} Cli;

Cli cli_init(const char* name, const char* description, int argc, char** argv);
void cli_add_arg(Cli* cli, Arg* arg);
Arg* cli_find_arg(Cli* cli, Arg* arg);
void cli_free(Cli* cli);
void cli_dump_help(Cli* cli);

#endif