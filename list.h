#ifndef __LINKED_LIST__H__
#define __LINKED_LIST__H__

#include <stddef.h>

typedef enum {
  LIST_ARG,
  LIST_ENTRY
} ListKind;

typedef struct ListNode {
  struct ListNode* next;
  void* data;
} ListNode;

typedef struct  {
  ListNode* head;
  ListNode* tail;
  ListKind kind;
} List;

ListNode* list_node_new(void* data);
void list_node_free(ListNode* node);

List* list_new(ListKind kind);
void list_append(List* list, ListNode* node);
void list_free(List* list);
void list_dump(List* list);

#endif