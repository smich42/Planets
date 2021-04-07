#include "HUDComponent.h"

HUDComponent::HUDComponent(sf::RenderWindow& window, sf::View& view) : window(window), view(view)
{}

HUDComponent::HUDComponent(sf::Vector2f pos, sf::RenderWindow& window, sf::View& view) : pos(pos), window(window), view(view)
{}

void HUDComponent::setName(std::string name)
{
    this->name = name;
}
