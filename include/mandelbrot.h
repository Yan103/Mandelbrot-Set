#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <assert.h>
#include <SFML/Graphics.hpp>
#include <immintrin.h>

#include "Tools.h"

const int           WIDTH                  = 1200;       //* int because int arifmetic next :)
const int           HEIGHT                 = 800;

const unsigned int  MAX_ITERATIONS         = 256;
const float         MAX_RADIUS_SQR         = 100.0f;

const unsigned int  X_OFFSET_CONSTANT      = 50;         //* mb need fix?
const unsigned int  Y_OFFSET_CONSTANT      = 35;         //* mb need fix?
const float         ZOOM_CONSTANT          = 1.1f;
const float         FPS_TIME_CONSTANT      = 0.5f;

const int           DEFAULT_X_CENTER       = 0;
const int           DEFAULT_Y_CENTER       = 0;
const float         DEFAULT_ZOOM           = 200.0f;

const float         CONSTANT_FOR_RED       = 9;
const float         CONSTANT_FOR_GREEN     = 15;
const float         CONSTANT_FOR_BLUE      = 8.5;
const int           TWO_HUNDRED_FIFTY_FIVE = 255;

const unsigned int  FONT_SIZE              = 20;
const float         TEXT_POSITION          = 10;

void KeyboardPressAction(sf::RenderWindow* window, int* x_center, int* y_center, float* zoom);

void MadelbrotSlowRealization(unsigned char* pixels, int x_center, int y_center, float zoom);

void MadelbrotFastRealization(unsigned char* pixels, int x_center, int y_center, float zoom);

ReturnCodes DrawMandelbrot(const char* font_name);

#endif // MANDELBROT_H
