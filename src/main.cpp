#include <assert.h>
#include <SFML/Graphics.hpp>
#include <string.h>

#include "Tools.h"
#include "mandelbrot.h"

//TODO
////1) Почему все в одном файле
////2) Убери относительные пути в инклюдах (флаг -I)
////3) У тебя константы могут быть отрицательными? Тогда зачем они int?
////4) Почему цвета захардкожены в код?
////5) Надо уменьшить вложенность 
////6) Почему функция называется Start Drawing? Че бы не просто Draw?
////7) MaGiC ConStAnTssssss
////8) Где аргументы командной строки? Добавь --help, надо чтобы файл который Arial.ttf можно было в командной строке указывать.

const char* DEFAULT_FONT_NAME = "arial.ttf";

int main(int argc, char* argv[]) {
    if (argc == 1) return DrawMandelbrot(DEFAULT_FONT_NAME);
    else {
        if        (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
            printf("My Mandelbrot set visualization\n"
                   "Options:\n"
                   "  --help (-h)       Show this help message\n"
                   "  --font (-f) FONT  Specify font file to use\n"
                   "If you run without flags, the program will run with the standard font (arial.ttf)\n");

            return SUCCESS;
        } else if (strcmp(argv[1], "--font") == 0 || strcmp(argv[1], "-f") == 0) {
            if (argc < 3) {
                fprintf(stderr, RED("Error: --font requires argument\n"));

                return ERROR;
            } else {
                return DrawMandelbrot(argv[2]);
            }
        } else {
            fprintf(stderr, RED("Unknown option: %s\n"), argv[1]);

            return ERROR;
        }
    }
}