#include "mandelbrot.h"

static ReturnCodes GetColorByIteration(unsigned char* color, float res_for_color) {
    assert(color != NULL && "Null pointer [color] was passed!\n");

    color[0] = 9    * (1 - res_for_color) *      res_for_color  *      res_for_color  * res_for_color * 255;
    color[1] = 15   * (1 - res_for_color) * (1 - res_for_color) *      res_for_color  * res_for_color * 255;
    color[2] = 8.5  * (1 - res_for_color) * (1 - res_for_color) * (1 - res_for_color) * res_for_color * 255;

    return SUCCESS;
}

static ReturnCodes SetColor(unsigned char* pixels, unsigned char* color, int index) {
    assert(pixels != NULL && "Null pointer [pixels] was passed!\n");
    assert(color  != NULL && "Null pointer [color] was passed!\n");

    pixels[index    ] = color[0];
    pixels[index + 1] = color[1];
    pixels[index + 2] = color[2];
    pixels[index + 3] = color[3];

    return SUCCESS;
}

ReturnCodes MandelbrotSlowRealization(unsigned char* pixels, int x_center, int y_center, float zoom) {
    assert(pixels != NULL && "Null pointer [pixels] was passed!\n"); 

    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            float x0 = (x -  WIDTH / 2 + x_center) / zoom;      //* start coordinates
            float y0 = (y - HEIGHT / 2 + y_center) / zoom;

            float xn = 0.0f, yn = 0.0f;
            float x2 = 0.0f, y2 = 0.0f, xy = 0.0f;  
            
            unsigned int iteration = 0;
            for (; iteration < MAX_ITERATIONS; ++iteration) {
                xn = x2 - y2 + x0;
                yn = 2.0f * xy + y0;

                x2 = xn * xn;
                y2 = yn * yn;
                xy = xn * yn;

                if (x2 + y2 >= MAX_RADIUS_SQR) {
                    break;
                }
            }

            unsigned char color[4] = {0, 0, 0, 255};            //* Default black color
            if (iteration < MAX_ITERATIONS) {
                float res_for_color = (float)iteration / MAX_ITERATIONS;
                GetColorByIteration(color, res_for_color);
            }

            SetColor(pixels, color, (y * WIDTH + x) * 4);
        }
    }

    return SUCCESS;
}

ReturnCodes MandelbrotFastRealization(unsigned char* pixels, int x_center, int y_center, float zoom) {
    assert(pixels != NULL && "Null pointer [pixels] was passed!\n"); 

    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; x += 8) {
            __m256 x0 = _mm256_set_ps(((x + 7) - WIDTH  / 2 + x_center) / zoom,
                                      ((x + 6) - WIDTH  / 2 + x_center) / zoom,
                                      ((x + 5) - WIDTH  / 2 + x_center) / zoom,
                                      ((x + 4) - WIDTH  / 2 + x_center) / zoom,
                                      ((x + 3) - WIDTH  / 2 + x_center) / zoom,
                                      ((x + 2) - WIDTH  / 2 + x_center) / zoom,
                                      ((x + 1) - WIDTH  / 2 + x_center) / zoom,
                                      ((x + 0) - WIDTH  / 2 + x_center) / zoom);
            __m256 y0 = _mm256_set1_ps((y      - HEIGHT / 2 + y_center) / zoom);

            __m256 xn = _mm256_set1_ps(0), yn = _mm256_set1_ps(0);
            __m256 x2 = _mm256_set1_ps(0), y2 = _mm256_set1_ps(0), xy = _mm256_set1_ps(0);
            
            __m256i _1         = _mm256_set1_epi32(1);
            __m256i iterations = _mm256_setzero_si256();

            for (int iter = 0; iter < MAX_ITERATIONS; ++iter) {
                xn = _mm256_add_ps(_mm256_sub_ps(x2, y2), x0);
                yn = _mm256_add_ps(_mm256_add_ps(xy, xy), y0);

                x2 = _mm256_mul_ps(xn, xn);
                y2 = _mm256_mul_ps(yn, yn);
                xy = _mm256_mul_ps(xn, yn);
                                                                                                  //* a < b macro!
                __m256 cmp = _mm256_cmp_ps(_mm256_add_ps(x2, y2), _mm256_set1_ps(MAX_RADIUS_SQR), _CMP_LT_OQ);
                unsigned int mask   = _mm256_movemask_ps(cmp);
                if (!mask) break;
                                                          //* 0 or 1 after cmp check
                iterations = _mm256_add_epi32(iterations, _mm256_and_si256(_mm256_castps_si256(cmp), _1));
            }

            unsigned int iter_counts[8] = {};
            _mm256_storeu_si256((__m256i*)iter_counts, iterations);

            for (int i = 0; i < 8; ++i) {
                int index = (y * WIDTH + x + i) * 4;

                unsigned char color[4] = {0, 0, 0, 255};
                if (iter_counts[i] < MAX_ITERATIONS) {
                    const float t = (float)iter_counts[i] / MAX_ITERATIONS;
                    GetColorByIteration(color, t);
                }

                SetColor(pixels, color, index);
            }
        }
    }

    return SUCCESS;
}

ReturnCodes KeyboardPressAction(sf::RenderWindow* window, int* x_center, int* y_center, float* zoom) {
    assert(window   != NULL && "Null pointer [window] was passed!\n"); 
    assert(x_center != NULL && "Null pointer [x_center] was passed!\n"); 
    assert(y_center != NULL && "Null pointer [y_center] was passed!\n"); 
    assert(zoom     != NULL && "Null pointer [zoom] was passed!\n"); 
 
    sf::Event event;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window->close();
            return SUCCESS;
        }      
        else if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::Right:
                *x_center += X_OFFSET_CONSTANT;
                return SUCCESS;

            case sf::Keyboard::Left:
                *x_center -= X_OFFSET_CONSTANT;
                return SUCCESS;

            case sf::Keyboard::Up:
                *y_center -= Y_OFFSET_CONSTANT;
                return SUCCESS;

            case sf::Keyboard::Down:
                *y_center += Y_OFFSET_CONSTANT;
                return SUCCESS;

            case sf::Keyboard::Dash:    //* "-" keyboard
                *zoom     /= ZOOM_CONSTANT;
                return SUCCESS;

            case sf::Keyboard::Equal:   //* "=" keyboard
                *zoom     *= ZOOM_CONSTANT;
                return SUCCESS;

            case sf::Keyboard::Escape:
                window->close();
                return SUCCESS;

            default:
                fprintf(stderr, RED("Unknown operation!\n"));
                return ERROR;  
            }               
        }            
    }

    return ERROR;
}

ReturnCodes DrawMandelbrot(const char* font_name, CalculateFunction CalcMandelbrotFunc) {
    assert(font_name          != NULL && "Null pointer [font_name] was passed!\n");
    assert(CalcMandelbrotFunc != NULL && "Null pointer [CalcMandelbrotFunc] was passed!\n");

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Mandelbrot Set");
                                                                    
    //* R|G|B and A (прозрачность)
    unsigned char* pixels = (unsigned char*)calloc(WIDTH * HEIGHT * 4, sizeof(unsigned char));
    if (!pixels) {
        fprintf(stderr, RED("Memory error!\n"));

        return ERROR;
    }

    sf::Texture texture;
    if (!texture.create(WIDTH, HEIGHT)) {
        fprintf(stderr, RED("Failed to create texture!\n"));
        FREE(pixels);

        return ERROR;
    }
    texture.update(pixels);
    sf::Sprite sprite(texture);

    int x_center = DEFAULT_X_CENTER;
    int y_center = DEFAULT_Y_CENTER;
    float   zoom = DEFAULT_ZOOM;            //* default parameters (TODO fix it?)

    sf::Font font;
    if (!font.loadFromFile(font_name)) { 
        fprintf(stderr, RED("Failed to load font!\n"));
        FREE(pixels);

        return ERROR;
    }

    sf::Text fpsText;
    fpsText.setFont(font);
    fpsText.setCharacterSize(FONT_SIZE);
    fpsText.setFillColor(sf::Color::White);
    fpsText.setPosition(TEXT_POSITION, TEXT_POSITION);

    sf::Clock clock;  
    int frameCount = 0;
    float timeAccumulator = 0.0f;

    while (window.isOpen()) {          
        float deltaTime = clock.restart().asSeconds();
        timeAccumulator += deltaTime;
        frameCount++;

        if (timeAccumulator >= FPS_TIME_CONSTANT) {
            float fps = frameCount / timeAccumulator;
            fpsText.setString("FPS: " + std::to_string((int)fps));
            timeAccumulator = 0.0f;
            frameCount = 0;
        }

        KeyboardPressAction(&window, &x_center, &y_center, &zoom);

        CalcMandelbrotFunc(pixels, x_center, y_center, zoom);

        texture.update(pixels);
        window.clear();
        window.draw(sprite);
        window.draw(fpsText);
        window.display();
    }

    FREE(pixels);

    return SUCCESS;
}
