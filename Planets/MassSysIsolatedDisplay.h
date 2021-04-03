#pragma once
#include "MassSysIsolated.h"
#include <SFML/Graphics.hpp>
#include <mutex>

class MassSysIsolatedDisplay
{
public:
    static const double MIN_DISPLAYABLE;

    bool running;

    MassSysIsolated& msi;
    std::mutex* mu;

    MassSysIsolatedDisplay(MassSysIsolated& msi, std::mutex* mu);

    void start();
    void stop();

    std::vector<sf::CircleShape> getCircles();
};

