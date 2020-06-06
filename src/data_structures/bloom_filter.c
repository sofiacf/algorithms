#include "./bloom_filter.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "../hashing/hash_functions.h"

const size_t N_ERROR = SIZE_MAX;
static const size_t byte_size = 8;
static const char* module_name = "BloomFilter";

typedef struct BloomFilter {
  size_t n;
  size_t bits;
  size_t funcs;
  char* filter;
} BloomFilter;

static bool is_power_2(size_t n) { return (ceil(log2(n)) == floor(log2(n))); }

BloomFilter* BloomFilter_Create(size_t bits, size_t funcs) {
  if (bits < byte_size || !is_power_2(bits) || funcs == 0 ||
      funcs > hasher_count) {
    ERROR(module_name, ArgumentOutOfRange);
    return NULL;
  }

  BloomFilter* filter = malloc(sizeof(BloomFilter));
  if (filter == NULL) {
    ERROR(module_name, FailedMemoryAllocation);
    return NULL;
  }

  filter->filter = calloc(1, bits / byte_size);
  if (filter->filter == NULL) {
    BloomFilter_Destroy(filter);
    ERROR(module_name, FailedMemoryAllocation);
    return NULL;
  }

  filter->n = 0;
  filter->bits = bits;
  filter->funcs = funcs;

  return filter;
}

size_t BloomFilter_GetN(const BloomFilter* self) {
  if (self == NULL) {
    ERROR(module_name, NullParameter);
    return N_ERROR;
  }

  return self->n;
}

ResultCode BloomFilter_Insert(BloomFilter* self, const char* key) {
  if (self == NULL || key == NULL) return NullParameter;

  size_t len = strlen(key);

  for (size_t i = 0; i < self->funcs; i++) {
    hash hash = hashers[i]((void*)key, len);
    size_t index = mul_compressor64(hash, self->bits);
    size_t byte_index = index / byte_size;
    size_t bit_index = index % byte_size;
    self->filter[byte_index] |= 1 << bit_index;
  }

  self->n++;
  return Success;
}

bool BloomFilter_Lookup(const BloomFilter* self, const char* key) {
  if (self == NULL || key == NULL) {
    ERROR(module_name, NullParameter);
    return false;
  }

  size_t len = strlen(key);

  for (size_t i = 0; i < self->funcs; i++) {
    hash hash = hashers[i]((void*)key, len);
    size_t index = mul_compressor64(hash, self->bits);
    size_t byte_index = index / byte_size;
    size_t bit_index = index % byte_size;

    if (!(self->filter[byte_index] & 1 << bit_index)) return false;
  }

  return true;
}

void BloomFilter_Destroy(BloomFilter* self) {
  free(self->filter);
  free(self);
}