#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef enum {
  Stack_kNullParameter = -2,
  Stack_FailedMalloc = -1,
  Stack_kSuccess = 0
} StackResult;

typedef struct Stack_Item {
  void* payload;
  struct Stack_Item* next;
} Stack_Item;

typedef struct Stack {
  size_t n;
  Stack_Item* head;
} Stack;

Stack* Stack_Create();
StackResult Stack_Push(Stack*, void*);
void* Stack_Pop(Stack*);
bool Stack_IskEmpty(Stack*);
void Stack_Destroy(Stack*);
