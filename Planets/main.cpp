#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <future>
#include <mutex>
#include <memory>
#include <SFML/Graphics.hpp>
#include <thread>
#include <Windows.h>
#include "Vec2.h"
#include "MassiveBody.h"
#include "MassSysIsolated.h"
#include "SolarSysData.h"
#include "MassSysIsolatedDisplay.h"

#define WIN_WIDTH 960
#define WIN_HEIGHT 640

#define M_PER_PX (1.0e9 / WIN_WIDTH)

void handleResizeEvent(sf::RenderWindow* window, sf::Event e)
{
    if (e.type == sf::Event::Resized)
    {
        sf::View view(sf::FloatRect(0, 0, e.size.width, e.size.height));
        view.setCenter(window->getView().getCenter());

        window->setView(view);
    }
}

void handleScrollEvent(sf::RenderWindow* window, sf::Event e)
{
    if (e.type == sf::Event::MouseWheelScrolled)
    {
        sf::View view = window->getView();

        float delta = e.mouseWheelScroll.delta;
        double zoomFactor = 0.1;

        view.zoom(delta == 1.0f ? 1.0 - zoomFactor : 1.0 + zoomFactor);

        window->setView(view);
    }
}

void panScreen(sf::RenderWindow* window, sf::Vector2f offset)
{
    sf::View view = window->getView();

    view.move(offset.x, offset.y);

    window->setView(view);
}

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Planets!", sf::Style::Default, settings);

    window.setView(sf::View(sf::Vector2f(0, 0), sf::Vector2f(WIN_WIDTH, WIN_HEIGHT)));
    window.setVerticalSyncEnabled(true);

    MassiveBody sun(SolarSysBody::SUN, Vec2(-149.55e9, 0));
    MassiveBody earth(SolarSysBody::EARTH, Vec2(0, 0));
    MassiveBody moon(SolarSysBody::LUNA, Vec2(0.384402e9, 0));
    MassiveBody mars(SolarSysBody::MARS, Vec2(265.6e9, 0));

    moon.attemptOrbit(earth, true);

    MassSysIsolated msi({ earth,  moon });

    std::mutex mu;

    MassSysIsolatedDisplay msiDisplay(msi, &mu);
    msiDisplay.start();

    //std::future posUpdater = std::async(
    //    [&window, &msiDisplay]() {
    //        while (window.isOpen())
    //        {
    //            msiDisplay.mu->lock();
    //            msiDisplay.msi.budgeAll();
    //            msiDisplay.mu->unlock();
    //        }
    //    });

    sf::Vector2f mousePosPrev;

    while (window.isOpen())
    {
        sf::Event e;

        while (window.pollEvent(e))
        {
            switch (e.type)
            {
            case sf::Event::Closed:
                msiDisplay.stop();
                window.close();
                break;

            case sf::Event::Resized:
                handleResizeEvent(&window, e);
                break;

            case sf::Event::MouseWheelScrolled:
                handleScrollEvent(&window, e);
                break;

            case sf::Event::MouseButtonPressed:
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                {
                    mousePosPrev = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                }
                break;

            case sf::Event::MouseMoved:
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                {
                    sf::Vector2f mousePosCur = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    sf::Vector2f offset = mousePosPrev - mousePosCur;

                    if (sqrt(offset.x * offset.x + offset.y * offset.y) < 10)
                    {
                        break;
                    }

                    panScreen(&window, offset);

                    mousePosPrev = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                }
                break;
            }
        }

        window.clear();

        for (sf::CircleShape circle : msiDisplay.getCircles())
        {
            window.draw(circle);
        }

        window.display();
    }

    return 0;
}