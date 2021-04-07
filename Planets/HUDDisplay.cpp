#include "HUDDisplay.h"
#include <iostream>

HUDDisplay::HUDDisplay(sf::RenderWindow& window, sf::View& view) : Display(window, view)
{}

HUDDisplay::HUDDisplay(sf::RenderWindow& window, sf::View& view, sf::Font& font) : Display(window, view, font)
{}

void HUDDisplay::addComponent(HUDComponent* component)
{
    component->font = this->font;
    components.emplace_back(component);
}

void HUDDisplay::processMouseEvent(sf::Event& eMouse)
{
    sf::View oldView = this->window.getView();
    this->window.setView(this->view);

    for (HUDComponent* component : components)
    {
        if (component != nullptr)
        {
            component->processMouseEvent(eMouse);
        }
    }

    this->window.setView(oldView);
}

void HUDDisplay::drawAll(DrawingOptions& options)
{
    sf::View oldView = this->window.getView();
    this->window.setView(this->view);

    for (HUDComponent* component : this->components)
    {
        if (component != nullptr)
        {
            component->draw(options);
        }
    }

    this->window.setView(oldView);
}
