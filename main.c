#include <stdio.h>
#include <stddef.h>
#include "cli.h"
#include "arg.h"

int main(int argc, char** argv) { 
  Cli cli = cli_init(NULL, NULL, argc, argv);

  cli_add_arg(&cli, arg_string("-f", "--file", "The name of the file", "file.txt"));
  cli_add_arg(&cli, arg_bool("-d"  , "--depth", "The depth of the file", false));
  cli_add_arg(&cli, arg_bool_required("-b"  , "--bool", "The bool flag"));
  cli_add_arg(&cli, arg_int("-c", "", "The counter", 0));

  cli_dump_help(&cli);

  List* args = cli_parse(&cli);

  printf("-f: %s\n", cli_get_string(&cli, "-f"));
  printf("-d: %s\n", cli_get_bool(&cli, "-d") ? "TRUE" : "FALSE");
  printf("-b: %s\n", cli_get_bool(&cli, "-b") ? "TRUE" : "FALSE");
  printf("-c: %d\n", *cli_get_int(&cli, "-c"));

  cli_free(&cli);
  list_free(args);

  return 0;
}