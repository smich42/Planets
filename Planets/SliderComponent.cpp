#include "SliderComponent.h"
#include <iostream>
#include <sstream>

SliderComponent::SliderComponent(double minValue, double maxValue, sf::RenderWindow& window, sf::View& view) : HUDComponent(window, view)
{
    this->foregroundColour = sf::Color::Red;

    this->minValue = minValue;
    this->maxValue = maxValue;

    this->dimMax = sf::Vector2f(300, 15);
    this->pos = sf::Vector2f(5, 5);

    this->setLevel(0.5f);
}

SliderComponent::SliderComponent(double minValue, double maxValue, sf::Vector2f pos, sf::RenderWindow& window, sf::View& view) : HUDComponent(pos, window, view)
{
    this->foregroundColour = sf::Color::Red;

    this->minValue = minValue;
    this->maxValue = maxValue;

    this->dimMax = sf::Vector2f(300, 15);

    this->setLevel(0.5f);
}

void SliderComponent::setForegroundColour(sf::Color colour)
{
    this->foregroundColour = colour;
}

float SliderComponent::getValue()
{
    return this->minValue + this->level * (this->maxValue - this->minValue);
}

void SliderComponent::setValue(double value)
{
    this->setLevel((value - this->minValue) / (this->maxValue - this->minValue));
}

void SliderComponent::setUnit(std::string unit)
{
    this->unit = unit;
}

void SliderComponent::setLevel(float level)
{
    this->level = level;
    this->foregroundRect.setSize(sf::Vector2f(this->dimMax.x * level, this->dimMax.y));
}

void SliderComponent::processMouseEvent(sf::Event& eMouse)
{
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(this->window));

    if (backgroundRect.getGlobalBounds().contains(mousePos) &&
        sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        this->setLevel(abs(mousePos.x - this->pos.x) / this->dimMax.x);
    }
}

void SliderComponent::draw(DrawingOptions& options)
{
    this->backgroundRect.setFillColor(sf::Color(240, 240, 240));
    this->foregroundRect.setFillColor(this->foregroundColour);

    this->backgroundRect.setPosition(sf::Vector2f(this->pos));
    this->foregroundRect.setPosition(sf::Vector2f(this->pos));

    this->backgroundRect.setSize(this->dimMax);

    this->window.draw(backgroundRect);
    this->window.draw(foregroundRect);

    sf::Text labelLeft;
    sf::Text labelRight;
    sf::Text labelName;

    labelLeft.setFont(this->font);
    labelRight.setFont(this->font);
    labelName.setFont(this->font);

    labelLeft.setCharacterSize(14);
    labelRight.setCharacterSize(14);
    labelName.setCharacterSize(14);

    labelLeft.setFillColor(sf::Color::White);
    labelRight.setFillColor(sf::Color::White);
    labelName.setFillColor(sf::Color::White);

    labelLeft.setOutlineColor(sf::Color::Black);
    labelRight.setOutlineColor(sf::Color::Black);
    labelName.setOutlineColor(sf::Color::Black);

    labelLeft.setOutlineThickness(2);
    labelRight.setOutlineThickness(2);
    labelName.setOutlineThickness(2);

    std::stringstream minValueSS;
    minValueSS << minValue << ' ' << this->unit;

    std::stringstream maxValueSS;
    maxValueSS << maxValue << ' ' << this->unit;

    labelLeft.setString(minValueSS.str());
    labelRight.setString(maxValueSS.str());
    labelName.setString(this->name);

    float xAfter = this->pos.x + this->backgroundRect.getGlobalBounds().width;
    float yBelow = this->pos.y + this->backgroundRect.getGlobalBounds().height + 5.0f;

    float labelRightW = labelRight.getGlobalBounds().width;

    labelLeft.setPosition(this->pos.x, yBelow);
    labelRight.setPosition(xAfter - labelRightW, yBelow);
    labelName.setPosition(xAfter + 10.0f, this->pos.y);

    this->window.draw(labelLeft);
    this->window.draw(labelRight);
    this->window.draw(labelName);
}
