#ifndef TESTER_H
#define TESTER_H

#include <x86intrin.h> 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "Tools.h"
#include "mandelbrot.h"

#define WARMUP_COUNT 10

typedef struct __TestCase{
    int   arg1;
    int   arg2;
    float arg3;
} TestCase;

ReturnCodes MandelbrotTestingFunction(CalculateFunction CalculateFunc, const char* tests_file, const char* results_file);

#endif // TESTER_H