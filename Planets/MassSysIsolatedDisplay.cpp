#include "MassSysIsolatedDisplay.h"
#include <future>
#include <Windows.h>

const double MassSysIsolatedDisplay::MIN_DISPLAYABLE = 1.0e5;

MassSysIsolatedDisplay::MassSysIsolatedDisplay(MassSysIsolated& msi, std::mutex* mu)
    : msi(msi), mu(mu)
{
    this->running = false;
}

void MassSysIsolatedDisplay::start()
{
    this->running = true;

    auto posUpdater = std::async(std::launch::async,
        [this]() {
            while (this->running)
            {
                this->mu->lock();
                this->msi.budgeAll();
                this->mu->unlock();
                // Sleep(10);
            }
        });
}

void MassSysIsolatedDisplay::stop()
{
    this->running = false;
}

std::vector<sf::CircleShape> MassSysIsolatedDisplay::getCircles()
{
    std::vector<sf::CircleShape> circles = {};

    for (MassiveBody& mb : this->msi.mbs)
    {
        sf::CircleShape circle(mb.getSIRadius() / MassSysIsolatedDisplay::MIN_DISPLAYABLE);

        Vec2 pos = mb.getSIPos() / MassSysIsolatedDisplay::MIN_DISPLAYABLE;

        circle.setPosition(pos.x, pos.y);

        Colour colour = SOLAR_SYS_SI_COLOURS.at(mb.type);

        circle.setFillColor(sf::Color(colour.r, colour.g, colour.b));
        circles.emplace_back(circle);
    }

    return circles;
}
