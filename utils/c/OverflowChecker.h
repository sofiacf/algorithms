#pragma once

#include <stddef.h>
#include <stdbool.h>

bool is_mul_overflow_size_t(size_t x, size_t y);
bool is_add_overflow_size_t(size_t x, size_t y);