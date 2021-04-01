#include <iostream>
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include "Vec2.h"
#include "MassiveBody.h"
#include "SolarSysData.h"

#define WIN_WIDTH 960
#define WIN_HEIGHT 540

#define M_PER_PX (1.0e9 / 960)

int main(int argc, char* args[])
{
    //std::cout << "[CPP standard: " << __cplusplus << "]" << std::endl;
    double earthX = (WIN_WIDTH / 2) * M_PER_PX;
    double earthY = (WIN_HEIGHT / 2) * M_PER_PX;

    MassiveBody earth("Earth",
                      SOLAR_SYS_SI_MASSES.at(SolarSysBody::EARTH),
                      SOLAR_SYS_SI_RADII.at(SolarSysBody::EARTH),
                      Vec2(earthX, earthY));

    MassiveBody moon("Luna",
                     SOLAR_SYS_SI_MASSES.at(SolarSysBody::LUNA),
                     SOLAR_SYS_SI_RADII.at(SolarSysBody::LUNA),
                     Vec2(385000e3, earthY));

    sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Planets!");

    sf::CircleShape earthCirc(earth.getSIRadius() / M_PER_PX);
    sf::CircleShape moonCirc(moon.getSIRadius() / M_PER_PX);

    earthCirc.setFillColor(sf::Color::Blue);
    moonCirc.setFillColor(sf::Color::White);

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        earthCirc.setPosition(earth.getSIPos().x / M_PER_PX,
                              earth.getSIPos().y / M_PER_PX);

        moonCirc.setPosition(moon.getSIPos().x / M_PER_PX,
                             moon.getSIPos().y / M_PER_PX);

        window.clear();

        window.draw(earthCirc);
        window.draw(moonCirc);

        window.display();

        moon.orbit(earth, false, 5);
    }

    return 0;
}