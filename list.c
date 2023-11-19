#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "list.h"
#include "cli.h"
#include "arg.h"

List* list_new(ListKind kind) {
  List* list = malloc(sizeof(*list));
  assert(list != NULL);
  
  *list = (List) {
    .head = NULL,
    .tail = NULL,
    .kind = kind,
    .count = 0
  };

  return list;
}

ListNode* list_node_new(void* data) {
  ListNode* node = malloc(sizeof(*node));
  assert(node != NULL);

  node->data = data;
  node->next = NULL;

  return node;
}

void list_append(List* list, ListNode* node) {
  assert(list != NULL);
  assert(node != NULL);

  if (!list->head && !list->head) {
    list->head = node;
    list->tail = node;
    list->head->next = NULL;
    list->tail->next = NULL;
  } else {
    if (!list->head->next) {
      list->head->next = node;
    } else {
      list->tail->next = node;
    }

    list->tail = node;
  }
}

Arg* list_find_arg(List *list, ListNode* node) {
  ListNode* temp_node = list->head;
  Arg* temp_arg = temp_node->data;
  Arg* arg = node->data;

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

Entry* list_find_entry(List *list, ListNode* node) {
  assert(false && "TODO: implement list_find_entry");
  return NULL;
}

ListNode* list_find(List* list, ListNode* node) {
  if (!list || !node || !list->head) {
    return NULL;
  }

  if (list->kind == LIST_ARG) {
    return list_node_new(list_find_arg(list, node));
  } else if (list->kind == LIST_ENTRY) {
    return list_node_new(list_find_entry(list, node));
  }

  return NULL;
}

void list_arg_dump(Arg* arg) {
  printf(" %s", arg->short_name);

  if (arg->long_name && *arg->long_name != '\0') {
    printf(", %s", arg->long_name);
  }

  if (arg->value) {
    printf("=");
    arg_print_value(arg);
  }

  if (arg->required) {
    printf(" \033[1m[required]\033[0m\n");
  }

  if (arg->description) {
    printf("\n    %s", arg->description);
  }

  printf("\n\n");
}

void list_dump(List* list) {
  ListNode* node = list->head;

  while (node) {
    if (list->kind == LIST_ARG) {
      list_arg_dump((Arg*)node->data);
    } else if (list->kind == LIST_ENTRY) {
      print_entry((Entry*)node->data);
    }

    node = node->next;
  }
}

void entry_free(Entry* entry) {
  free(entry->value);
  free(entry);
}

void list_free(List* list) {
  ListNode* current = list->head;
  ListNode* temp = NULL;

  while (current) {
    temp = current;
    current = current->next;

    if (list->kind == LIST_ARG) {
      arg_free((Arg*)temp->data);
    } else if (list->kind == LIST_ENTRY) {
      entry_free((Entry*)temp->data);
    }

    free(temp);
  }

  free(list);
}