#ifndef TESTER_H
#define TESTER_H

#include "Tools.h"
#include "mandelbrot.h"

typedef struct __TestCase{
    int   arg1;
    int   arg2;
    float arg3;
} TestCase;

ReturnCodes MandelbrotTestingFunction(CalculateFunction CalculateFunc, const char* tests_file, const char* results_file);

#endif // TESTER_H