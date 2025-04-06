#include "tester.h"

const size_t TEST_COUNT = 5;

ReturnCodes MandelbrotTestingFunction(CalculateFunction CalculateFunc, const char* tests_file_name, const char* results_file_name) {
    assert(CalculateFunc     != NULL && "Null pointer [CalculateFunction]!");
    assert(tests_file_name   != NULL && "Null pointer [tests_file_name]!");
    assert(results_file_name != NULL && "Null pointer [results_file_name]!");
    
    unsigned char* pixels = (unsigned char*)calloc(WIDTH * HEIGHT * 4, sizeof(unsigned char));
    if (pixels == NULL) {
        fprintf(stderr, RED("Memory error!\n"));
        return ERROR;
    }

    FILE* tests_file = fopen(tests_file_name, "r");
    if (tests_file == NULL) {
        FREE(pixels);
        return ERROR;
    }
    
    FILE* results_file = fopen(results_file_name, "w");
    if (results_file == NULL) {
        fclose(tests_file);
        FREE(pixels);
        return ERROR;
    }

    size_t num_tests = 0;
    if (fscanf(tests_file, "%lu", &num_tests) != 1) {
        fclose(tests_file);
        fclose(results_file);
        FREE(pixels);
        return ERROR;
    }

    TestCase* tests = (TestCase*)calloc(num_tests, sizeof(TestCase));
    if (tests == NULL) {
        fclose(tests_file);
        fclose(results_file);
        FREE(pixels);
        return ERROR;
    }

    for (size_t i = 0; i < num_tests; i++) {
        if (fscanf(tests_file, "%d %d %f", &tests[i].arg1, &tests[i].arg2, &tests[i].arg3) != 3) {
            FREE(tests);
            FREE(pixels);
            fclose(tests_file);
            fclose(results_file);
            return ERROR;
        }
    }
    fclose(tests_file);

    //* cash warmup before main tests
    for (size_t warmup = 0; warmup < WARMUP_COUNT; warmup++) {
        CalculateFunc(pixels, tests[0].arg1, tests[0].arg2, tests[0].arg3);
    }
    printf(GREEN("Warmup end!\n"));

    uint64_t start = 0;
    for (size_t i = 0; i < num_tests; i++) {        
        for (size_t run = 0; run < TEST_COUNT; run++) {
            start = __rdtsc();
            
            CalculateFunc(pixels, tests[i].arg1, tests[i].arg2, tests[i].arg3);
            
            fprintf(results_file, "Test %lu [Run %lu]: %.3lld tacts\n", i + 1, run + 1, __rdtsc() - start);
        }
    }

    FREE(pixels);
    FREE(tests);
    fclose(results_file);
    
    return SUCCESS;
}
