#pragma once

#include <stddef.h>

#include "ListOperations.h"

typedef struct {
    comparator comparator;
    size_t n;
    size_t item_size;
    void* array;
} Array;

Array* Array_Create(comparator, size_t item_size);
ListOpResult Array_Insert(Array*, const void*);
void* Array_Search(const Array*, const void*);
ListOpResult Array_Enumerate(const Array*, item_handler);
void Array_Destroy(Array*);