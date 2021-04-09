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
#include "SliderComponent.h"

#define BACKGROUND_COLOUR sf::Color(5, 15, 30)

#define WIN_WIDTH 960
#define WIN_HEIGHT 640

#define WIN_MARGIN 15

#define MAX_ZOOM (float)5.0e4
#define MIN_ZOOM (1.0f)

void handleResizeEvent(sf::RenderWindow& window, sf::View& planetsView, sf::View& guiView, const sf::Event& e, const float& resultantZoom)
{
    if (e.type == sf::Event::Resized)
    {
        planetsView = sf::View(sf::FloatRect(0, 0, e.size.width, e.size.height));
        guiView = sf::View(sf::FloatRect(0, 0, e.size.width, e.size.height));

        planetsView.setCenter(window.getView().getCenter());
        planetsView.zoom(resultantZoom);

        window.setView(planetsView);
    }
}

void handleScrollEvent(sf::RenderWindow& window, sf::View& planetsView, const sf::Event& e, float& resultantZoom)
{
    if (e.type == sf::Event::MouseWheelScrolled)
    {
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

        planetsView.zoom(zoom);

        window.setView(planetsView);
    }
}

void panScreen(sf::RenderWindow& window, sf::View& planetsView, const sf::Vector2f& offset)
{
    planetsView.move(offset.x, offset.y);
    window.setView(planetsView);
}

void drawFPS(sf::RenderWindow& window, int fps, float resultantZoom)
{
    sf::Font fpsFont;
    sf::Text fpsLabel;

    fpsFont.loadFromFile("fonts/Ubuntu/Ubuntu-Medium.ttf");
    fpsLabel.setFont(fpsFont);

    fpsLabel.setCharacterSize(16);
    fpsLabel.setFillColor(sf::Color(70, 230, 50));
    fpsLabel.setScale(resultantZoom, resultantZoom);

    fpsLabel.setString(std::to_string(fps) + " fps");

    sf::Vector2f pos = window.mapPixelToCoords(sf::Vector2i(window.getSize().x - WIN_MARGIN, WIN_MARGIN));

    fpsLabel.setPosition(pos.x - fpsLabel.getGlobalBounds().width, pos.y);

    window.draw(fpsLabel);
}

void simulate()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 10;

    float resultantZoom = 1.0f;

    sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Planets!", sf::Style::Default, settings);

    sf::View planetsView = sf::View(sf::Vector2f(0, 0), sf::Vector2f(WIN_WIDTH, WIN_HEIGHT));
    sf::View guiView = sf::View(sf::Vector2f(WIN_WIDTH / 2, WIN_HEIGHT / 2), sf::Vector2f(WIN_WIDTH, WIN_HEIGHT));

    window.setView(planetsView);
    //window.setVerticalSyncEnabled(true);

    MassiveBody sun(SolarSysBody::SUN, Vec2(0, 0));
    MassiveBody earth(SolarSysBody::EARTH, Vec2(152e9, 0));
    MassiveBody moon(SolarSysBody::LUNA, Vec2(152e9 + 0.384402e9, 0));
    MassiveBody mars(SolarSysBody::MARS, Vec2(152e9 + 54.6e9, 0));

    //earth.attemptOrbit(sun, true);
    //mars.attemptOrbit(sun, true);
    moon.attemptOrbit(earth, true);

    MassSysIsolated msi({ sun, earth,  moon, mars });
    MassSysIsolatedDisplay msiDisplay(msi, window, planetsView);

    HUDDisplay hudDisplay(window, guiView);

    SliderComponent updateIntervalSlider = SliderComponent(0.005, 0.02, sf::Vector2f(WIN_MARGIN, WIN_MARGIN), window, guiView);
    updateIntervalSlider.setForegroundColour(sf::Color(140, 35, 70));
    updateIntervalSlider.setValue(0.01);
    updateIntervalSlider.setName("Physics update frequency (prefer high)");
    updateIntervalSlider.setUnit("MHz");

    SliderComponent speedFactorSlider = SliderComponent(1, 100000, sf::Vector2f(WIN_MARGIN, WIN_MARGIN + 50.0f), window, guiView);
    speedFactorSlider.setForegroundColour(sf::Color(35, 140, 70));
    speedFactorSlider.setValue(1000);
    speedFactorSlider.setName("Simulation speed multiplier (prefer low)");

    hudDisplay.addComponent(&updateIntervalSlider);
    hudDisplay.addComponent(&speedFactorSlider);

    sf::Vector2f mousePosPrev;
    bool panning = false;

    sf::Clock physicsClock;
    sf::Clock fpsMeasureClock;
    sf::Clock fpsUpdateClock;

    sf::Time accumulator = sf::microseconds(0);
    sf::Time physicsTime = sf::microseconds(1.0f / updateIntervalSlider.getValue());

    bool debug = false;
    unsigned int fps = 0;
    unsigned int frameCount = 0;

    while (window.isOpen())
    {
        physicsTime = sf::microseconds(1.0f / updateIntervalSlider.getValue());

        accumulator += physicsClock.getElapsedTime();
        physicsClock.restart();

        while (accumulator >= physicsTime)
        {
            msiDisplay.msi.budgeAll(physicsTime.asSeconds() * speedFactorSlider.getValue());
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
                handleResizeEvent(window, planetsView, guiView, e, resultantZoom);
                break;

            case sf::Event::MouseWheelScrolled:
                handleScrollEvent(window, planetsView, e, resultantZoom);
                break;

            case sf::Event::MouseButtonPressed:
                hudDisplay.processMouseEvent(e);

                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                {
                    panning = true;

                    //sf::Cursor cursor;
                    //cursor.loadFromSystem(sf::Cursor::SizeAll);
                    //window.setMouseCursor(cursor);

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

                    //sf::Cursor cursor;
                    //cursor.loadFromSystem(sf::Cursor::Arrow);
                    //window.setMouseCursor(cursor);
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

                    panScreen(window, planetsView, offset);
                    mousePosPrev = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                }
                break;
            }
        }

        window.clear(BACKGROUND_COLOUR);

        DrawingOptions options = { 15, resultantZoom, debug };

        msiDisplay.drawAll(options);
        hudDisplay.drawAll(options);

        if (debug)
        {
            drawFPS(window, fps, resultantZoom);

            if (fpsUpdateClock.getElapsedTime() >= sf::seconds(1.0f))
            {
                fps = round(frameCount / fpsMeasureClock.getElapsedTime().asSeconds());

                frameCount = 0;
                fpsUpdateClock.restart();
                fpsMeasureClock.restart();
            }
        }

        ++frameCount;
        window.display();
    }
}

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    simulate();

    return 0;
}