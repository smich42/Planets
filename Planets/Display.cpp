#include "Display.h"

Display::Display(sf::RenderWindow& window) : window(window)
{}

Display::Display(sf::RenderWindow& window, sf::Font font) : window(window), font(font)
{}

void Display::drawFloatRect(sf::FloatRect fRect, sf::Vector2f pos, sf::Color colour)
{
    sf::RectangleShape globalBoundsRect;

    globalBoundsRect.setSize(sf::Vector2f(fRect.width, fRect.height));
    globalBoundsRect.setPosition(pos);

    globalBoundsRect.setOutlineColor(sf::Color(0, 0, 0, 0));
    globalBoundsRect.setFillColor(colour);

    window.draw(globalBoundsRect);
}

void Display::drawBoundsFor(sf::RectangleShape shape)
{
    this->drawFloatRect(shape.getGlobalBounds(), shape.getPosition(), sf::Color(255, 255, 0, 128));
}

void Display::drawBoundsFor(sf::CircleShape shape)
{
    this->drawFloatRect(shape.getGlobalBounds(), shape.getPosition(), sf::Color(0, 255, 25, 128));
}

void Display::drawBoundsFor(sf::Text text)
{
    this->drawFloatRect(text.getGlobalBounds(), text.getPosition(), sf::Color(210, 50, 235, 128));
}
