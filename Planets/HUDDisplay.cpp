#include "HUDDisplay.h"

HUDDisplay::HUDDisplay(sf::RenderWindow& window) : Display(window)
{}

HUDDisplay::HUDDisplay(sf::RenderWindow& window, sf::Font font) : Display(window, font)
{}

SliderDisplayable HUDDisplay::makeSpeedSlider(DrawingOptions options)
{
    sf::RectangleShape sliderBackground;
    sliderBackground.setSize(sf::Vector2f(50, 10));
    sliderBackground.setScale(options.zoom, options.zoom);
    sliderBackground.setFillColor(sf::Color::White);

    sf::Vector2f pos = this->window.mapPixelToCoords(
        sf::Vector2i(this->window.getSize().x - sliderBackground.getGlobalBounds().width, 0));

    return { sliderBackground };
}

void HUDDisplay::drawAll(DrawingOptions& options)
{
    SliderDisplayable sd = this->makeSpeedSlider(options);

    this->window.draw(sd.sliderBackground);
    if (options.debug) this->drawBoundsFor(sd.sliderBackground);
}
