#include <assert.h>
#include <SFML/Graphics.hpp>
#include <string.h>
#include <unistd.h>

#include "Tools.h"
#include "mandelbrot.h"
#include "tester.h"

const char* DEFAULT_FONT_NAME = "arial.ttf";

int main(int argc, char* argv[]) {
    int opt = 0, flag = 0;

    while ((opt = getopt(argc, argv, "hfdot")) != -1) {
        switch (opt) {
            case 'h':
                printf("My Mandelbrot set visualization\n"
                       "Options:\n"
                       "  -h        Show this help message\n"
                       "  -f [FONT] Specify font file to use\n"
                       "  -o        Launch of optimized version\n"
                       "  -d        Launch of slow version\n"
                       "If you run without flags, the program will run with the standard font (arial.ttf)\n");
                flag = 1;
                break;

            case 'f':
                return DrawMandelbrot(optarg, MandelbrotFastRealization);
            
            case 'o':
                return DrawMandelbrot(DEFAULT_FONT_NAME, MandelbrotFastRealization);
            
            case 'd':
                return DrawMandelbrot(DEFAULT_FONT_NAME, MandelbrotSlowRealization);
            
            case 't':
                flag = 1;

                printf(GREEN("Start tests...\n"));
                MandelbrotTestingFunction(MandelbrotSlowRealization, "tests/tests.txt", "tests/slow_results.txt");
                printf(GREEN("All tests passed successfully\n"));
                break;

            default:
                fprintf(stderr, RED("Unknown option: -%c\n"), optopt);
                return ERROR;
        }
    }

    if (!flag) {
        return DrawMandelbrot(DEFAULT_FONT_NAME, MandelbrotFastRealization);
    }
}
