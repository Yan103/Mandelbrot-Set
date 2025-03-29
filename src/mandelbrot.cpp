#include "mandelbrot.h"

static void GetColorByIteration(unsigned char* color, float res_for_color) {
    assert(color != NULL && "Null pointer was passed!\n");

    color[0] = CONSTANT_FOR_RED   * (1 - res_for_color) *      res_for_color  *      res_for_color  * res_for_color * TWO_HUNDRED_FIFTY_FIVE;
    color[1] = CONSTANT_FOR_GREEN * (1 - res_for_color) * (1 - res_for_color) *      res_for_color  * res_for_color * TWO_HUNDRED_FIFTY_FIVE;
    color[2] = CONSTANT_FOR_BLUE *  (1 - res_for_color) * (1 - res_for_color) * (1 - res_for_color) * res_for_color * TWO_HUNDRED_FIFTY_FIVE;
}

static void SetColor(unsigned char* pixels, unsigned char* color, int index) {
    assert(pixels != NULL && color != NULL && "Null pointer was passed!\n");

    pixels[index    ] = color[0];
    pixels[index + 1] = color[1];
    pixels[index + 2] = color[2];
    pixels[index + 3] = color[3];
}

void MadelbrotSlowRealization(unsigned char* pixels, int x_center, int y_center, float zoom) {
    assert(pixels != NULL && "Null pointer was passed!\n"); 

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

            unsigned char color[4] = {0, 0, 0, TWO_HUNDRED_FIFTY_FIVE};            //* Default black color
            if (iteration < MAX_ITERATIONS) {
                float res_for_color = (float)iteration / MAX_ITERATIONS;
                GetColorByIteration(color, res_for_color);
            }

            SetColor(pixels, color, (y * WIDTH + x) * 4);
        }
    }
}

void KeyboardPressAction(sf::RenderWindow* window, int* x_center, int* y_center, float* zoom) {
    assert(window != NULL && x_center != NULL && y_center != NULL && zoom != NULL && "Null pointer was passed!\n");

    sf::Event event;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window->close();
        } 
        else if (event.type == sf::Event::KeyPressed) {         //* ya mb ne pon, no vot'
        switch (event.key.code) {
            case sf::Keyboard::Right:
                *x_center += X_OFFSET_CONSTANT;
                break;
            case sf::Keyboard::Left:
                *x_center -= X_OFFSET_CONSTANT;
                break;
            case sf::Keyboard::Up:
                *y_center -= Y_OFFSET_CONSTANT;
                break;
            case sf::Keyboard::Down:
                *y_center += Y_OFFSET_CONSTANT;
                break;
            case sf::Keyboard::Dash:    //* "-" keyboard
                *zoom     /= ZOOM_CONSTANT;
                break;
            case sf::Keyboard::Equal:   //* "=" keyboard
                *zoom     *= ZOOM_CONSTANT;
                break;
            case sf::Keyboard::Escape:
                window->close();
                break;
            default:
                break;                  //* error check?!?
            }            
        }
    }
}

ReturnCodes DrawMandelbrot(const char* font_name) {
    assert(font_name != NULL && "Null pointer was passed!\n");

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

        MadelbrotSlowRealization(pixels, x_center, y_center, zoom);

        texture.update(pixels);
        window.clear();
        window.draw(sprite);
        window.draw(fpsText);
        window.display();
    }

    FREE(pixels);

    return SUCCESS;
}
