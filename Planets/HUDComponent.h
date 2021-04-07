#pragma once
#include <SFML/Graphics.hpp>
#include "Display.h"

class HUDComponent
{
protected:
    sf::Vector2f pos;
    sf::Vector2f dimMax;

    std::string name;

    sf::RenderWindow& window;
    sf::View& view;

public:
    sf::Font font;

    HUDComponent(sf::RenderWindow& window, sf::View& view);
    HUDComponent(sf::Vector2f pos, sf::RenderWindow& window, sf::View& view);

    void setName(std::string name);

    virtual void processMouseEvent(sf::Event& eMouse) = 0;
    virtual void draw(DrawingOptions& options) = 0;
};

