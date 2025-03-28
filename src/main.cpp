#include <assert.h>
#include <SFML/Graphics.hpp>
#include "../include/Tools.h"

const int   WIDTH             = 1200;
const int   HEIGHT            = 800;
const int   MAX_ITERATIONS    = 256;
const int   X_OFFSET_CONSTANT = 50;     //* mb need fix?
const int   Y_OFFSET_CONSTANT = 35;     //* mb need fix?
const float MAX_RADIUS_SQR    = 100.0f;
const float ZOOM_CONSTANT     = 1.1f;
const float FPS_TIME_CONSTANT = 0.5f;

void MadelbrotSlowRealization(unsigned char* pixels, int x_center, int y_center, float zoom) {
    assert(pixels != NULL); 

    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            float x0 = (x -  WIDTH / 2 + x_center) / zoom;      //* start coordinates
            float y0 = (y - HEIGHT / 2 + y_center) / zoom;

            float xn = 0.0f, yn = 0.0f;
            float x2 = 0.0f, y2 = 0.0f, xy = 0.0f;  
            
            int iteration = 0;
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
                
                //* it will be optimize)
                color[0] =   9 * (1 - res_for_color) * res_for_color * res_for_color * res_for_color * 255;
                color[1] =  15 * (1 - res_for_color) * (1 - res_for_color) * res_for_color * res_for_color * 255;
                color[2] = 8.5 * (1 - res_for_color) * (1 - res_for_color) * (1 - res_for_color) * res_for_color * 255;
            }

            int index         = (y * WIDTH + x) * 4;
            pixels[index]     = color[0];
            pixels[index + 1] = color[1];
            pixels[index + 2] = color[2];
            pixels[index + 3] = color[3];
        }
    }
}

void KeyboardPressAction(sf::RenderWindow* window, int* x_center, int* y_center, float* zoom) {
    assert(window != NULL && x_center != NULL && y_center != NULL && zoom != NULL);

    sf::Event event;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window->close();
        } else if (event.type == sf::Event::KeyPressed) {
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
                default:
                    break;                  //* error check?!?
            }            
        }
    }
}

ReturnCodes StartDrawMandelbrot() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Mandelbrot Set");
    //window.setFramerateLimit(60);
                                                                    //* R|G|B and A (прозрачность)
    unsigned char* pixels = (unsigned char*)calloc(WIDTH * HEIGHT * 4, sizeof(unsigned char));
    if (!pixels) {
        fprintf(stderr, RED("MEMORY ERROR!\n"));

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

    int x_center = 0, y_center = 0;
    float zoom = 200.0f;                //* defaul parameters (TODO fix it?)

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) { 
        fprintf(stderr, RED("Failed to load font!\n"));
        FREE(pixels);

        return ERROR;
    }

    sf::Text fpsText;
    fpsText.setFont(font);
    fpsText.setCharacterSize(20);
    fpsText.setFillColor(sf::Color::White);
    fpsText.setPosition(10, 10);

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

int main() {
    StartDrawMandelbrot();

    return SUCCESS;
}