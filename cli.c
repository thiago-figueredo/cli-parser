#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "helpers.h"
#include "list.h"
#include "cli.h"
#include "arg.h"

Cli cli_init(const char* name, const char* description, int argc, char** argv) {
  assert(argv != NULL);

  return (Cli) {
    .name = name ? name : get_filename(argv[0]),
    .description = description,
    .arg_list = list_new(LIST_ARG),
    .argc = argc,
    .argv = argv
  };
}

void panic(Cli* cli, Arg* arg) {
  cli_free(cli);
  arg_free(arg);
  exit(EXIT_FAILURE);
}

void cli_add_arg(Cli* cli, Arg* arg) {
  assert(cli != NULL);
  assert(arg != NULL);

  if (!arg->short_name && !arg->long_name) {
    fprintf(stderr, "ERROR: arg must have a short name or a long name\n");
    panic(cli, arg);
  }

  if (!arg->long_name && arg->short_name && strlen(trim_str(arg->short_name)) == 0) {
    fprintf(stderr, "ERROR: arg short name must not contains only white spaces\n");
    panic(cli, arg);
  }

  if (!arg->short_name && arg->long_name && strlen(trim_str(arg->long_name)) == 0) {
    fprintf(stderr, "ERROR: arg long name must not contains only white spaces\n");
    panic(cli, arg);
  }

  Arg* existing_arg = cli_find_arg(cli, arg);

  // Throw an error because, if we set the existing_arg the caller must free the previous arg passed
  if (existing_arg) {
    if (arg->short_name){
      fprintf(stderr, "ERROR: %s is already registered\n", arg->short_name);
      panic(cli, arg);
    } else {
      fprintf(stderr, "ERROR: %s is already registered\n", arg->long_name);
      panic(cli, arg);
    }
  }

  list_append(cli->arg_list, list_node_new(arg));
}

Arg* cli_find_arg(Cli* cli, Arg* arg) {
  if (!cli || !arg || !arg->short_name || !arg->long_name || !cli->arg_list || !cli->arg_list->head) {
    return false;
  }

  ListNode* temp_node = cli->arg_list->head;
  Arg* temp_arg = temp_node->data;

  while (temp_node) {
    if (temp_arg->short_name == arg->short_name) {
      return temp_arg;
    }

    if (temp_arg->long_name && strcmp(temp_arg->long_name, arg->long_name) == 0) {
      return temp_arg;
    }

    temp_node = temp_node->next;
  }

  return NULL;
}

Arg* cli_find_arg_by_name(Cli* cli, const char* arg_name) {
  if (!cli || !arg_name) {
    return false;
  }

  ListNode* temp_node = cli->arg_list->head;

  while (temp_node) {
    Arg* temp_arg = temp_node->data;

    if (strcmp(temp_arg->short_name, arg_name) == 0) {
      return temp_arg;
    }

    if (temp_arg->long_name && strcmp(temp_arg->long_name, arg_name) == 0) {
      return temp_arg;
    }

    temp_node = temp_node->next;
  }

  return NULL;
}

char* cli_get_string(Cli* cli, const char* key) {
  assert(cli != NULL);
  assert(key != NULL);

  Arg* arg = cli_find_arg_by_name(cli, key);

  if (arg) {
    return (char*)arg->value;
  }

  return NULL;
}

bool* cli_get_bool(Cli* cli, const char* key) {
  assert(cli != NULL);
  assert(key != NULL);

  Arg* arg = cli_find_arg_by_name(cli, key);

  if (arg) {
    return (bool*)arg->value;
  }

  return NULL;
}

int* cli_get_int(Cli* cli, const char* key) {
  assert(cli != NULL);
  assert(key != NULL);

  Arg* arg = cli_find_arg_by_name(cli, key);

  if (arg) {
    return (int*)arg->value;
  }

  return NULL;
}

Entry* entry_new(const char* key, void* value, EntryKind kind) {
  Entry* entry = malloc(sizeof(*entry));
  assert(entry != NULL);

  *entry = (Entry) {
    .key = key,
    .value = value,
    .kind = kind,
  };

  return entry;
}

void cli_parse_free_panic(Cli* cli, List* args) {
  cli_free(cli);
  list_free(args);
  exit(EXIT_FAILURE);
}

List* cli_parse(Cli* cli) {
  assert(cli != NULL);

  List* args = list_new(LIST_ENTRY);

  for (int i = 1; i < cli->argc; ++i) {
    char* str = cli->argv[i];
    Arg* arg = cli_find_arg_by_name(cli, str);

    if (arg && i+1 < cli->argc) { 
      void* value;

      if (arg->kind == ARG_STRING) {
        value = malloc(sizeof(char) * strlen(cli->argv[i+1]) + 1);
        assert(value != NULL);
        assert(strcpy(value, cli->argv[++i]) != NULL);
      } else if (arg->kind == ARG_INT) {
        value = malloc(sizeof(int));
        assert(value != NULL);
        *(int*)value = atoi(cli->argv[++i]);
      } else if (arg->kind == ARG_BOOL) {
        value = malloc(sizeof(bool));
        assert(value != NULL);
        *(bool*)value = false;

        if (strcmp(cli->argv[i+1], "true") == 0) {
          *(bool*)value = true;
          i++;
        } else if (strcmp(cli->argv[i+1], "false") == 0) {
          i++;
        } else {
          fprintf(stderr, "ERROR: Unknown boolean value `%s` \n", cli->argv[i+1]);
          free(value);
          cli_parse_free_panic(cli, args);
        }
      } else {
        fprintf(stderr, "ERROR: Unknown arg kind\n");
        cli_parse_free_panic(cli, args);
      }

      arg->value = value;

      Entry* entry = entry_new(str, value, cli->arg_list->kind);
      assert(entry != NULL);
      list_append(args, list_node_new(entry));
    } else if (!arg) {
      fprintf(stderr, "ERROR: Unknown option `%s`\n", str);
      list_free(cli->arg_list);
      list_free(args);
      exit(EXIT_FAILURE);
    }
  }

  return args;
}

void print_entry(Entry* entry) {
  if (!entry) return;

  printf("%s => ", entry->key);

  if (entry->kind == ENTRY_STRING) {
    printf("%s", (char*)entry->value);
  } else if (entry->kind == ENTRY_INT) {
    printf("%d", *(int*)entry->value);
  } else if (entry->kind == ENTRY_BOOL) {
    printf("%s", *(bool*)entry->value ? "true" : "false");
  }

  printf("\n");
}

void cli_dump_help(Cli* cli) {
  if (!cli) return;

  Arg* arg = cli->arg_list->head->data;

  if (cli->description) {
    printf("%s - %s\n\n", cli->name, cli->description);
  }

  printf("USAGE: %s [OPTIONS]\n", cli->name);

  list_dump(cli->arg_list);
}

void cli_free(Cli* cli) {
  list_free(cli->arg_list);
}