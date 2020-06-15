// Copyright 2020 Dale Alleshouse
#pragma once

#include <inttypes.h>

#include "./array.h"
#include "./binary_tree.h"
#include "./linked_list.h"
#include "./list_operations.h"

uintptr_t _enumerate_sum;

typedef enum {
  kInsert = 1,
  kSearch = 2,
  kEnumerate = 3,
  kMax = 4,
  kPredecessor = 5,
  kSelect = 6,
  kRank = 7
} Operation;

typedef enum {
  kArray = 1,
  kSortedArray = 2,
  kLinkedList = 3,
  kLinkedListPoorLocality = 4,
  kBinaryTree = 5,
  kBinaryTreeUnbalanced = 6,
  kRedBlackTree = 7
} Structure;

typedef ListOpResult (*ListOp)(void*, uintptr_t);
typedef uintptr_t (*SearchOp)(const void*, const uintptr_t);
typedef void (*EnumerateOp)(const void*);
typedef uintptr_t (*MaxOp)(const void*);
typedef uintptr_t (*PredOp)(const void*, const uintptr_t);
typedef uintptr_t (*SelectOp)(const void*, const size_t);
typedef size_t (*RankOp)(const void*, const uintptr_t);

ListOp GetInsertOperation(Structure);
SearchOp GetSearchOperation(Structure);
EnumerateOp GetEnumerateOperation(Structure);

void* BuildkEmptyDataStructure(Structure);
void* BuildDataStructure(Structure, size_t);
void DestroyStructure(Structure, void*);

int ptr_comparator(const void*, const void*);
double OperationTime(Operation, Structure, size_t);
