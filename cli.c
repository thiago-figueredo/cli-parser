#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "helpers.h"
#include "cli.h"
#include "arg.h"

Cli cli_init(const char* name, const char* description, int argc, char** argv) {
  assert(argv != NULL);

  return (Cli) {
    .name = name ? name : get_filename(argv[0]),
    .description = description,
    .args_head = NULL,
    .args_tail = NULL,
    .argc = argc,
    .argv = argv
  };
}

void cli_add_arg(Cli* cli, Arg* arg) {
  assert(cli != NULL);
  assert(arg != NULL);

  if (!arg->short_name && !arg->long_name) {
    fprintf(stderr, "ERROR: arg must have a short name or a long name\n");
    exit(EXIT_FAILURE);
  }

  if (!arg->long_name && arg->short_name && strlen(trim_str(arg->short_name)) == 0) {
    fprintf(stderr, "ERROR: arg short name must not contains only white spaces\n");
    exit(EXIT_FAILURE);
  }

  if (!arg->short_name && arg->long_name && strlen(trim_str(arg->long_name)) == 0) {
    fprintf(stderr, "ERROR: arg long name must not contains only white spaces\n");
    exit(EXIT_FAILURE);
  }

  Arg* existing_arg = cli_find_arg(cli, arg);

  // Throw an error because, if we set the existing_arg the caller must free the previous arg passed
  if (existing_arg) {
    if (arg->short_name){
      fprintf(stderr, "ERROR: %s is already registered\n", arg->short_name);
    } else {
      fprintf(stderr, "ERROR: %s is already registered\n", arg->long_name);
    }

    exit(EXIT_FAILURE);
  }

  if (!cli->args_head && !cli->args_tail) {
    cli->args_head = arg;
    cli->args_tail = arg;
    cli->args_head->next = NULL;
    cli->args_tail->next = NULL;
  } else if (cli->args_tail) {
    if (!cli->args_head->next) {
      cli->args_head->next = arg;
    } else {
      cli->args_tail->next = arg;
    }

    cli->args_tail = arg;
  }
}

Arg* cli_find_arg(Cli* cli, Arg* arg) {
  if (!cli || !arg || !arg->short_name || !arg->long_name) {
    return false;
  }

  if (*arg->short_name == '\0' || !strlen(trim_str(arg->long_name))) {
    return false;
  }

  Arg* temp_arg = cli->args_head;

  while (temp_arg) {
    if (temp_arg->short_name == arg->short_name) {
      return temp_arg;
    }

    if (temp_arg->long_name && strcmp(temp_arg->long_name, arg->long_name) == 0) {
      return temp_arg;
    }

    temp_arg = temp_arg->next;
  }

  return false;
}

void cli_dump_help(Cli* cli) {
  Arg* arg = cli->args_head;

  if (cli->description) {
    printf("%s - %s\n\n", cli->name, cli->description);
  }

  printf("USAGE: %s [OPTIONS]\n", cli->name);

  while (arg) {
    printf(" %s", arg->short_name);

    if (arg->long_name && *arg->long_name != '\0') {
      printf(", %s", arg->long_name);
    }
  
    if (arg->value) {
      printf("=");
      arg_print_value(arg);
    }

    if (arg->required && !arg->value) {
      printf(" \033[1m[required]\033[0m\n");
    }

    if (arg->description) {
      printf("\n    %s", arg->description);
    }

    printf("\n\n");
    arg = arg->next;
  }
}

void cli_free(Cli* cli) {
  free_arg(cli->args_head);
}