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
#include "HUDDisplay.h"

#define BACKGROUND_COLOUR sf::Color(5, 10, 30)

#define WIN_WIDTH 960
#define WIN_HEIGHT 640

#define SCALING_THRESHOLD 40.0f

#define MAX_ZOOM (float)5.0e3
#define MIN_ZOOM (0.8f)

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

void drawFPS(sf::RenderWindow& window, int fpsCount, float resultantZoom)
{
    sf::Font fpsFont;
    sf::Text fps;

    fpsFont.loadFromFile("fonts/Ubuntu/Ubuntu-Medium.ttf");
    fps.setFont(fpsFont);

    fps.setCharacterSize(16);
    fps.setFillColor(sf::Color(70, 230, 50));
    fps.setScale(resultantZoom, resultantZoom);

    fps.setString(std::to_string(fpsCount) + " fps");

    sf::FloatRect fpsTextBounds = fps.getGlobalBounds();
    fps.setPosition(window.mapPixelToCoords(sf::Vector2i(0, 0)));

    window.draw(fps);
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
    MassSysIsolatedDisplay msiDisplay(msi, window);
    HUDDisplay hudDisplay(window);

    sf::Vector2f mousePosPrev;
    bool panning = false;

    bool debug = false;
    sf::Clock physicsClock;
    sf::Clock fpsClock;
    sf::Time accumulator = sf::microseconds(0);
    sf::Time physicsTime = sf::microseconds(100);

    while (window.isOpen())
    {
        accumulator += physicsClock.getElapsedTime();
        physicsClock.restart();

        while (accumulator >= physicsTime)
        {
            msiDisplay.msi.budgeAll(physicsTime.asSeconds() * 1000);
            accumulator -= physicsTime;
        }

        sf::Event e;

        while (window.pollEvent(e))
        {
            switch (e.type)
            {
            case sf::Event::Closed:
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

        msiDisplay.drawAll(options);
        hudDisplay.drawAll(options);

        if (debug)
        {
            int fpsCount = (int)1.0f / fpsClock.getElapsedTime().asSeconds();
            drawFPS(window, fpsCount, resultantZoom);
        }

        fpsClock.restart();

        window.display();
    }

    return 0;
}