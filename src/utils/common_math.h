/*******************************************************************************
 * Copyright (C) 2020 Dale Alleshouse (AKA Hideous Humpback Freak)
 *  dale@alleshouse.net https://hideoushumpbackfreak.com/
 *
 * This file is subject to the terms and conditions defined in the 'LICENSE'
 * file, which is part of this source code package.
 ******************************************************************************/
#pragma once

#include <limits.h>
#include <stddef.h>

#include "./comparators.h"

int Min(const size_t count, ...);
double MinDouble(const size_t count, ...);

void* MaxPArray(const size_t n, const size_t size, void* arr,
                const comparator comparator);
void* MinPArray(const size_t n, const size_t size, void* arr,
                const comparator comparator);

size_t NextPowerOfTwo(size_t n);
