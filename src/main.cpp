#include <assert.h>
#include <SFML/Graphics.hpp>
#include "../include/Tools.h"

const int   WIDTH             = 800;
const int   HEIGHT            = 600;
const int   MAX_ITERATIONS    = 256;
const int   X_OFFSET_CONSTANT = 50;
const int   Y_OFFSET_CONSTANT = 35;
const float MAX_RADIUS        = 100.0f;
const float ZOOM_CONSTANT     = 1.1f;

void MadelbrotSlowRealization(unsigned char* pixels, int x_center, int y_center, float zoom) {
    assert(pixels != NULL); 

    

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
                    *zoom           *= ZOOM_CONSTANT;
                    break;
                case sf::Keyboard::Equal:   //* "=" keyboard
                    *zoom           /= ZOOM_CONSTANT;
                    break;
                default:
                    break;                  //* error check?!?
            }            
        }
    }
}

ReturnCodes StartDrawMandelbrot() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Mandelbrot Set");
    window.setFramerateLimit(60);
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

    while (window.isOpen()) {
        KeyboardPressAction(&window, &x_center, &y_center, &zoom);

        MadelbrotSlowRealization(pixels, x_center, y_center, zoom);

        texture.update(pixels);
        window.clear();
        window.draw(sprite);
        window.display();
    }

    FREE(pixels);

    return SUCCESS;
}

int main() {
    StartDrawMandelbrot();

    /*while (window.isOpen()) {
        sf::Event event;

        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window->close();
        }

        for (unsigned y = 0; y < HEIGHT; y++) {
            for (unsigned x = 0; x < WIDTH; x++) {
                unsigned index = (y * WIDTH + x) * 4;

                if (x == y) {
                    pixels[index]     = 255;     // RED
                    pixels[index + 1] =   0;     // GREEN
                    pixels[index + 2] =   0;     // BLUE
                }

                pixels[index + 3]     = 255;     // ALPHA
            }
        }

        texture.update(pixels);
        window.clear();
        window.draw(sprite);
        window.display();
    }*/

    return SUCCESS;
}