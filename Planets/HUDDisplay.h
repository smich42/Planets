#pragma once
#include "Display.h"
#include "HUDComponent.h"
#include <SFML/Graphics.hpp>

class HUDDisplay : public Display
{
private:
    std::vector<HUDComponent*> components;

public:
    explicit HUDDisplay(sf::RenderWindow& window, sf::View& view);
    explicit HUDDisplay(sf::RenderWindow& window, sf::View& view, sf::Font& font);

    void addComponent(HUDComponent* component);

    void processMouseEvent(sf::Event& eMouse);
    void drawAll(DrawingOptions& options) override;
};

