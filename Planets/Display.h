#pragma once
#include <SFML/Graphics.hpp>

struct DrawingOptions
{
    int pxLabelYSpacing;
    float zoom;
    bool debug;
};

class Display
{
protected:
    sf::RenderWindow& window;
    sf::View& view;
    sf::Font font;

public:
    Display(sf::RenderWindow& window, sf::View& view);
    Display(sf::RenderWindow& window, sf::View& view, sf::Font& font);

    void drawFloatRect(sf::FloatRect fRect, sf::Vector2f pos, sf::Color colour);

    void drawBoundsFor(sf::RectangleShape shape);
    void drawBoundsFor(sf::CircleShape shape);
    void drawBoundsFor(sf::Text text);

    virtual void drawAll(DrawingOptions& options) = 0;
};