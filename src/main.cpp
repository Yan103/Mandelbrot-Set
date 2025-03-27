#include <SFML/Graphics.hpp>
#include "../include/Tools.h"

//! g++ src/main.cpp -l sfml-graphics -l sfml-window -l sfml-system



const unsigned WIDTH  = 500;
const unsigned HEIGHT = 500;

int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Mandelbrot Set");
    window.setFramerateLimit(60);
                                                                    //* R|G|B and A (прозрачность)
    unsigned char* pixels = (unsigned char*)calloc(WIDTH * HEIGHT * 4, sizeof(unsigned char));
    if (!pixels) {
        fprintf(stderr, "MEMORY ERROR!\n");

        return ERROR;
    }

    sf::Texture texture;
    if (!texture.create(WIDTH, HEIGHT)) {
        fprintf(stderr, "Failed to create texture!\n");
        FREE(pixels);

        return ERROR;
    }
    texture.update(pixels);
    sf::Sprite sprite(texture);

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        for (unsigned y = 0; y < HEIGHT; y++) {
            for (unsigned x = 0; x < WIDTH; x++) {
                unsigned index = (y * WIDTH + x) * 4;
                printf("%d\n", index);

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
    }

    FREE(pixels);

    return SUCCESS;
}