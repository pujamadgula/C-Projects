#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(200, 200), "Image Test");
    sf::Texture texture;

    if (!texture.loadFromFile("pieces/fir.png")) {
        std::cerr << "Failed to load texture from file." << std::endl;
        return 1;
    }

    sf::Sprite sprite(texture);
    sprite.setPosition(5, 10);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}
