#include "./heap.h"

#include <stdint.h>
#include <stdlib.h>

#include "../utils/overflow_checker.h"

static size_t _parentIndex(size_t index) {
  ++index;
  index = index >> 1;
  return index - 1;
}

static size_t _childIndex(size_t index) {
  index++;
  index = index << 1;
  return index - 1;
}

static HeapResult _setDataItem(Heap* self, void* item, size_t index) {
  // Clean up whatever is in the current slot
  void* existing = HashTable_Find(self->item_tracker, &item, sizeof(void*));
  free(existing);

  // Geta a new index
  size_t* i = malloc(sizeof(index));
  if (i == NULL) return HeapkFailedMemoryAllocation;

  // Set the index in the hash table
  *i = index;
  HashTable_Insert(self->item_tracker, &item, sizeof(void*), i);

  // Update the data
  self->data[index] = item;
  return HeapkSuccess;
}

static HeapResult _swap(Heap* self, size_t x, size_t y) {
  void* temp = self->data[x];
  HeapResult result = _setDataItem(self, self->data[y], x);
  if (result != HeapkSuccess) return result;

  result = _setDataItem(self, temp, y);
  if (result != HeapkSuccess) return result;
  /* data[x] = data[y]; */
  /* data[y] = temp; */
  return HeapkSuccess;
}

static HeapResult _bubbleUp(Heap* self, size_t start) {
  while (start > 0) {
    size_t parent_index = _parentIndex(start);
    int comp_result =
        self->comparator(self->data[start], self->data[parent_index]);

    if (comp_result <= 0) break;

    HeapResult result = _swap(self, start, parent_index);
    if (result != HeapkSuccess) return result;

    start = parent_index;
  }

  return HeapkSuccess;
}

static size_t _greatestPriority(Heap* self, size_t x, size_t y) {
  if (y >= self->n) return x;

  if (x >= self->n) return y;

  int result = self->comparator(self->data[x], self->data[y]);

  if (result >= 0) return x;

  return y;
}

static HeapResult _bubbleDown(Heap* self, size_t start) {
  size_t child;

  while ((child = _childIndex(start)) < self->n) {
    child = _greatestPriority(self, child, child + 1);

    int comp_result = self->comparator(self->data[start], self->data[child]);

    if (comp_result >= 0) return HeapkSuccess;

    HeapResult result = _swap(self, start, child);
    if (result != HeapkSuccess) return result;

    start = child;
  }

  return HeapkSuccess;
}

// Find a valid size for the hash table
static size_t _nextPowerOf2(size_t val) {
  size_t count = 0;

  if (val && !(val & (val - 1))) return val;

  while (val != 0) {
    val >>= 1;
    count += 1;
  }

  return 1 << count;
}

Heap* Heap_Create(size_t size, comparator comparator) {
  if (comparator == NULL) {
    HEAP_ERROR(HeapkNullParameter);
    return NULL;
  }

  if (size == 0) {
    HEAP_ERROR(HeapInvalidSize);
    return NULL;
  }

  if (is_mul_overflow_size_t(sizeof(void*), size)) {
    HEAP_ERROR(HeapkArithmeticOverflow);
    return NULL;
  }

  Heap* self = malloc(sizeof(Heap));

  if (self == NULL) {
    HEAP_ERROR(HeapkFailedMemoryAllocation);
    return NULL;
  }

  self->data = malloc(sizeof(void*) * size);

  if (self->data == NULL) {
    HEAP_ERROR(HeapkFailedMemoryAllocation);
    Heap_Destroy(self, NULL);
    return NULL;
  }

  self->item_tracker = HashTable_Create(_nextPowerOf2(size));
  if (self->item_tracker == NULL) {
    HEAP_ERROR(HeapHashTableError);
    Heap_Destroy(self, NULL);
    return NULL;
  }

  self->size = size;
  self->n = 0;
  self->comparator = comparator;

  return self;
}

void Heap_Destroy(Heap* self, freer freer) {
  if (self == NULL) return;

  HashTable_Destroy(self->item_tracker, free);

  if (freer != NULL && self->data != NULL) {
    for (size_t i = 0; i < self->n; i++) freer(self->data[i]);
  }

  free(self->data);
  free(self);
}

HeapResult Heap_Insert(Heap* self, void* item) {
  if (self == NULL || item == NULL) return HeapkNullParameter;

  if (self->size == self->n) return HeapOverflow;

  HeapResult result = _setDataItem(self, item, self->n);
  if (result != HeapkSuccess) return result;

  result = _bubbleUp(self, self->n);
  if (result != HeapkSuccess) return result;

  self->n++;

  return HeapkSuccess;
}

HeapResult Heap_Resize(Heap* self, size_t size) {
  if (self == NULL) return HeapkNullParameter;

  if (size < self->n) return HeapInvalidSize;

  if (is_mul_overflow_size_t(size, sizeof(void*))) {
    return HeapkArithmeticOverflow;
  }

  void* new_data = realloc(self->data, sizeof(void*) * size);
  if (new_data == NULL) return HeapkFailedMemoryAllocation;

  self->data = new_data;
  self->size = size;

  return HeapkSuccess;
}

void* Heap_Extract(Heap* self) {
  if (self == NULL) {
    HEAP_ERROR(HeapkNullParameter);
    return NULL;
  }

  if (Heap_IskEmpty(self)) {
    HEAP_ERROR(HeapkEmpty);
    return NULL;
  }

  void* item = self->data[0];
  self->n--;
  HeapResult result = _swap(self, 0, self->n);
  if (result != HeapkSuccess) {
    HEAP_ERROR(result);
  }

  result = _bubbleDown(self, 0);
  if (result != HeapkSuccess) {
    HEAP_ERROR(result);
  }

  return item;
}

void* Heap_Peek(Heap* self) {
  if (self == NULL) {
    HEAP_ERROR(HeapkNullParameter);
    return NULL;
  }

  if (Heap_IskEmpty(self)) {
    HEAP_ERROR(HeapkEmpty);
    return NULL;
  }

  return self->data[0];
}

bool Heap_IskEmpty(Heap* self) {
  if (self == NULL) {
    HEAP_ERROR(HeapkNullParameter);
    return true;
  }

  if (self->n == 0) return true;

  return false;
}

bool Heap_Exists(Heap* self, void* findMe) {
  if (self == NULL || findMe == NULL) {
    HEAP_ERROR(HeapkNullParameter);
    return false;
  }

  void* found = HashTable_Find(self->item_tracker, &findMe, sizeof(void*));

  return found != NULL;
}

HeapResult Heap_Reproiritize(Heap* self, void* item) {
  if (self == NULL || item == NULL) return HeapkNullParameter;

  size_t* index = HashTable_Find(self->item_tracker, &item, sizeof(void*));
  if (index == NULL) return HeapItemkNotFound;

  // Index is the first item in queue, so it can only go down
  if (*index == 0) return _bubbleDown(self, *index);

  // Figure out if item has a higher priority than it
  size_t parent_index = _parentIndex(*index);
  int comp_result =
      self->comparator(self->data[*index], self->data[parent_index]);

  // If they are equal, no need to move anything
  if (comp_result == 0) return HeapkSuccess;

  // If it is greater, then bubble it up
  if (comp_result > 0) return _bubbleUp(self, *index);

  // The only other option is less so bubble it down
  return _bubbleDown(self, *index);
}

char* Heap_ErrorMessage(HeapResult result) {
  switch (result) {
    case HeapItemkNotFound:
      return "The specified item was not found in the heap";
    case HeapkArithmeticOverflow:
      return "An operation caused an arithmetic overflow";
    case HeapOverflow:
      return "Attempted to place more items in the heap than it is sized for";
    case HeapkEmpty:
      return "Attempted operation on an empty heap";
    case HeapkFailedMemoryAllocation:
      return "Failed to allocate memory";
    case HeapInvalidSize:
      return "n must be greater than zero.";
    case HeapkNullParameter:
      return "One of the required parameters passed to the function is NULL";
    case HeapkSuccess:
      return "kSuccess";
    case HeapHashTableError:
      return "The HashTable object used to locate Heap items caused an error";
    default:
      return "Unknown error code";
  }
}
