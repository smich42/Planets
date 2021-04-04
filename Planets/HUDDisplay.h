#pragma once
#include "Display.h"
#include <SFML/Graphics.hpp>

struct SliderDisplayable
{
    sf::RectangleShape sliderBackground;
};

class HUDDisplay : public Display
{
private:
    SliderDisplayable makeSpeedSlider(DrawingOptions options);

public:
    explicit HUDDisplay(sf::RenderWindow& window);
    explicit HUDDisplay(sf::RenderWindow& window, sf::Font font);

    void drawAll(DrawingOptions& options) override;
};

