#pragma once

#include <stddef.h>

#include "include/CommonTypes.h"
#include "include/ResultCode.h"

const size_t INITAL_HEAP_SIZE;

typedef struct RunningMedian RunningMedian;

RunningMedian* RunningMedian_Create();
Result RunningMedian_Insert(RunningMedian*, double value);
double RunningMedian_Median(RunningMedian*);
size_t RunningMedian_GetN(RunningMedian*);
void RunningMedian_Destroy(RunningMedian*);