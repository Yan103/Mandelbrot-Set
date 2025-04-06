#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <SFML/Graphics.hpp>
#include <immintrin.h>

#include "Tools.h"

const int           WIDTH             = 1200;       //* int because int arifmetic next :)
const int           HEIGHT            = 800;

const unsigned int  MAX_ITERATIONS    = 256;
const float         MAX_RADIUS_SQR    = 100.0f;

const unsigned int  X_OFFSET_CONSTANT = 50;         //* mb need fix?
const unsigned int  Y_OFFSET_CONSTANT = 35;         //* mb need fix?
const float         ZOOM_CONSTANT     = 1.1f;
const float         FPS_TIME_CONSTANT = 0.5f;

const int           DEFAULT_X_CENTER  = 0;
const int           DEFAULT_Y_CENTER  = 0;
const float         DEFAULT_ZOOM      = 200.0f;

const unsigned int  FONT_SIZE         = 20;
const float         TEXT_POSITION     = 10;

typedef ReturnCodes (*CalculateFunction)(unsigned char* pixels, int x_center, int y_center, float zoom);

ReturnCodes KeyboardPressAction(sf::RenderWindow* window, int* x_center, int* y_center, float* zoom);

ReturnCodes MandelbrotSlowRealization(unsigned char* pixels, int x_center, int y_center, float zoom);

ReturnCodes MandelbrotFastRealization(unsigned char* pixels, int x_center, int y_center, float zoom);

ReturnCodes DrawMandelbrot(const char* font_name, CalculateFunction CalcMandelbrotFunc);

#endif // MANDELBROT_H
