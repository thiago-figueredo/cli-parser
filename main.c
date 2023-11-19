#include <stddef.h>
#include "cli.h"
#include "arg.h"

int main(int argc, char** argv) { 
  Cli cli = cli_init(NULL, NULL, argc, argv);

  cli_add_arg(&cli, arg_string("-f", "--file", "The name of the file", "file.txt"));
  cli_add_arg(&cli, arg_bool("-d"  , "--depth", "The depth of the file", false));
  cli_add_arg(&cli, arg_bool_required("-d2"  , "--depth2", "The depth of the file2"));
  cli_add_arg(&cli, arg_int("-c", "", "The counter", 0));

  cli_dump_help(&cli);
  cli_free(&cli);

  return 0;
}