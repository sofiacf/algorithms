#pragma once

// We may need a better way to log errors, but this will work for now
#define ERROR(source, result)                                                  \
    {                                                                          \
        char str[1000];                                                        \
        sprintf(str, "source: %s, %s, %s, %d\n", Result_ErrorMessage(result),  \
            __FILE__, __FUNCTION__, __LINE__);                                 \
                                                                               \
        ErrorReporter_Report(result, str);                                     \
    }

typedef enum Result {
    Empty = -7,
    DependancyError = -6,
    ArgumentOutOfRange = -5,
    InvalidIndex = -4,
    ArithmeticOverflow = -3,
    FailedMemoryAllocation = -2,
    NullParameter = -1,
    Success = 0
} Result;

char* Result_ErrorMessage(Result);
