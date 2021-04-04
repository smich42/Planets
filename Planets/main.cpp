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

#define BACKGROUND_COLOUR sf::Color(5, 10, 30)

#define WIN_WIDTH 960
#define WIN_HEIGHT 640

#define SCALING_THRESHOLD 40.0f

#define MAX_ZOOM 4.0e3
#define MIN_ZOOM (1.0f / MAX_ZOOM)

void handleResizeEvent(sf::RenderWindow& window, const sf::Event& e, const float& resultantZoom)
{
    if (e.type == sf::Event::Resized)
    {
        sf::View view(sf::FloatRect(0, 0, e.size.width, e.size.height));

        view.setCenter(window.getView().getCenter());
        view.zoom(resultantZoom);

        window.setView(view);
    }
}

void handleScrollEvent(sf::RenderWindow& window, const sf::Event& e, float& resultantZoom)
{
    if (e.type == sf::Event::MouseWheelScrolled)
    {
        sf::View view = window.getView();

        float delta = e.mouseWheelScroll.delta;

        if (delta == 0)
        {
            return;
        }

        float zoomFactor = 0.2f;
        float zoom = ((delta > 0) ? (1.0f - zoomFactor) : (1.0f + zoomFactor));

        float candidateZoom = resultantZoom * zoom;

        if (candidateZoom > MAX_ZOOM) zoom = MAX_ZOOM / resultantZoom;
        if (candidateZoom < MIN_ZOOM) zoom = MIN_ZOOM / resultantZoom;

        resultantZoom *= zoom;

        view.zoom(zoom);

        window.setView(view);
    }
}

void panScreen(sf::RenderWindow& window, const sf::Vector2f& offset)
{
    sf::View view = window.getView();

    view.move(offset.x, offset.y);
    window.setView(view);
}

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    float resultantZoom = 1.0f;

    sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Planets!", sf::Style::Default, settings);

    window.setView(sf::View(sf::Vector2f(0, 0), sf::Vector2f(WIN_WIDTH, WIN_HEIGHT)));
    window.setVerticalSyncEnabled(true);

    MassiveBody sun(SolarSysBody::SUN, Vec2(-152e9, 0));
    MassiveBody earth(SolarSysBody::EARTH, Vec2(0, 0));
    MassiveBody moon(SolarSysBody::LUNA, Vec2(0.384402e9, 0));
    MassiveBody mars(SolarSysBody::MARS, Vec2(54.6e9, 0));

    //earth.attemptOrbit(sun, true);
    //mars.attemptOrbit(sun, true);
    moon.attemptOrbit(earth, true);

    MassSysIsolated msi({ sun, earth,  moon, mars });
    MassSysIsolatedDisplay msiDisplay(msi);

    std::atomic<bool> posUpdaterRunning{ true };
    std::mutex mu;
    std::future posUpdater = std::async(std::launch::async, &MassSysIsolatedDisplay::calcPositions, msiDisplay, std::ref(posUpdaterRunning), &mu);

    sf::Vector2f mousePosPrev;
    bool panning = false;

    bool debug = false;

    while (window.isOpen())
    {
        sf::Event e;

        while (window.pollEvent(e))
        {
            switch (e.type)
            {
            case sf::Event::Closed:
                posUpdaterRunning = false;
                window.close();
                break;

            case sf::Event::Resized:
                handleResizeEvent(window, e, resultantZoom);
                break;

            case sf::Event::MouseWheelScrolled:
                handleScrollEvent(window, e, resultantZoom);
                break;

            case sf::Event::MouseButtonPressed:
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                {
                    panning = true;

                    sf::Cursor cursor;
                    cursor.loadFromSystem(sf::Cursor::SizeAll);
                    window.setMouseCursor(cursor);

                    mousePosPrev = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                }
                if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
                {
                    debug = !debug;
                }
                break;

            case sf::Event::MouseButtonReleased:
                if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
                {
                    panning = false;

                    sf::Cursor cursor;
                    cursor.loadFromSystem(sf::Cursor::Arrow);
                    window.setMouseCursor(cursor);
                }
                break;

            case sf::Event::MouseMoved:
                if (panning)
                {
                    sf::Vector2f mousePosCur = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    sf::Vector2f offset = mousePosPrev - mousePosCur;

                    if (sqrt(offset.x * offset.x + offset.y * offset.y) < 10)
                    {
                        break;
                    }

                    panScreen(window, offset);
                    mousePosPrev = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                }
                break;
            }
        }

        window.clear(BACKGROUND_COLOUR);

        DrawingOptions options = { 15, resultantZoom, SCALING_THRESHOLD, debug };
        sf::Font labelFont;

        msiDisplay.drawAll(window, options);

        window.display();
    }

    return 0;
}