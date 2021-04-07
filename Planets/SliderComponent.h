#pragma once
#include "HUDComponent.h"

class SliderComponent : public HUDComponent
{
private:
    sf::Color foregroundColour;
    sf::RectangleShape backgroundRect;
    sf::RectangleShape foregroundRect;

    std::string unit;
    double maxValue;
    double minValue;

    float level;

    void setLevel(float level);

public:
    SliderComponent(double minValue, double maxValue, sf::RenderWindow& window, sf::View& view);
    SliderComponent(double minValue, double maxValue, sf::Vector2f pos, sf::RenderWindow& window, sf::View& view);

    void setForegroundColour(sf::Color colour);

    float getValue();
    void setValue(double value);

    void setUnit(std::string unit);

    void processMouseEvent(sf::Event& eMouse) override;
    void draw(DrawingOptions& options) override;
};

