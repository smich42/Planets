#include "MassSysIsolatedDisplay.h"
#include <algorithm>
#include <future>
#include <Windows.h>
#include <iostream>

const double MassSysIsolatedDisplay::LENGTH_SCALE_DOWN_FACTOR = 1.0e-5;
const unsigned int MassSysIsolatedDisplay::TRAIL_SIZE_MAX = (unsigned int)1.0e4;
sf::Font MassSysIsolatedDisplay::font{};

MassSysIsolatedDisplay::MassSysIsolatedDisplay(MassSysIsolated& msi) : msi(msi)
{}

void MassSysIsolatedDisplay::calcPositions(std::atomic<bool>& running, std::mutex* mu)
{
    while (running.load())
    {
        mu->lock();
        this->msi.budgeAll();
        mu->unlock();
    }
}

std::vector<PlanetDrawable> MassSysIsolatedDisplay::getUnprocessedPlanetDrawables()
{
    std::vector<PlanetDrawable> planets = {};

    for (MassiveBody& mb : this->msi.mbs)
    {
        Vec2 pos = mb.getSIPos() * MassSysIsolatedDisplay::LENGTH_SCALE_DOWN_FACTOR;

        sf::CircleShape circle;
        this->initCircle(circle, mb, pos);
        this->initTrail(circle, mb, pos);

        sf::Text label;
        this->initLabel(label, mb, pos);

        sf::RectangleShape labelBackdrop;
        this->initLabelBackdrop(labelBackdrop, label, mb, pos);

        planets.emplace_back(PlanetDrawable{ circle, label, labelBackdrop, this->trails[mb.getID()] });
    }

    return planets;
}

sf::Color MassSysIsolatedDisplay::colourOf(MassiveBody& mb)
{
    Colour _c = SOLAR_SYS_SI_COLOURS.at(mb.type);
    return sf::Color(_c.r, _c.g, _c.b);
}

void MassSysIsolatedDisplay::initCircle(sf::CircleShape& circle, MassiveBody& mb, Vec2& pos)
{
    circle.setRadius(mb.getSIRadius() * MassSysIsolatedDisplay::LENGTH_SCALE_DOWN_FACTOR);
    circle.setPosition(pos.x, pos.y);
    circle.setFillColor(MassSysIsolatedDisplay::colourOf(mb));
}

void MassSysIsolatedDisplay::initTrail(sf::CircleShape& circle, MassiveBody& mb, Vec2& pos)
{
    unsigned int id = mb.getID();

    if (this->trails.find(id) == this->trails.end())
    {
        this->trails[id] = {};
    }

    sf::Vector2f trailPoint(pos.x + circle.getRadius(), pos.y + circle.getRadius());

    sf::Color colour = MassSysIsolatedDisplay::colourOf(mb);

    std::vector<sf::Vertex>& trailVertices = this->trails[id];
    trailVertices.emplace_back(sf::Vertex(trailPoint, colour));

    if (trailVertices.size() > TRAIL_SIZE_MAX)
    {
        trailVertices.erase(trailVertices.begin(), trailVertices.begin() + (trailVertices.size() - TRAIL_SIZE_MAX));
    }

    std::cout << trailVertices.size() << std::endl;
}

void MassSysIsolatedDisplay::initLabel(sf::Text& label, MassiveBody& mb, Vec2& pos)
{
    sf::Font& labelFont = MassSysIsolatedDisplay::font;

    labelFont.loadFromFile("fonts/Ubuntu/Ubuntu-Medium.ttf");

    label.setFont(labelFont);
    label.setString(mb.getName());
    label.setCharacterSize(14);
    //label.setFillColor(sf::Color::Black);
    label.setFillColor(sf::Color::White);
    label.setOutlineColor(sf::Color::Black);
    label.setOutlineThickness(2);
    label.setPosition(pos.x, pos.y);
}

void MassSysIsolatedDisplay::initLabelBackdrop(sf::RectangleShape& labelBackdrop, sf::Text& label, MassiveBody& mb, Vec2& pos)
{
    float labelW = label.getLocalBounds().width;
    float labelH = label.getLocalBounds().height;

    labelBackdrop.setSize(sf::Vector2f(labelW, labelH));
    labelBackdrop.setPosition(pos.x, pos.y);
    labelBackdrop.setOutlineColor(sf::Color(0, 0, 0, 0));
    //labelBackdrop.setFillColor(sf::Color(255, 255, 255, 128));
    labelBackdrop.setFillColor(sf::Color(0, 0, 0, 0));
}

std::vector<PlanetDrawable> MassSysIsolatedDisplay::getProcessedPlanetDrawables(const sf::RenderWindow& window, DrawingOptions& options)
{
    std::vector<PlanetDrawable> planetDrawables = this->getUnprocessedPlanetDrawables();

    for (PlanetDrawable& pd : planetDrawables)
    {
        this->applyZoom(pd, options);
    }

    this->formLabelStacks(planetDrawables, window, options);

    return planetDrawables;
}

void MassSysIsolatedDisplay::applyZoom(PlanetDrawable& pd, DrawingOptions& options)
{
    if (options.zoom > options.scalingThreshold)
    {
        float scale = options.zoom / options.scalingThreshold;
        pd.circle.setScale(scale, scale);
    }

    pd.label.setScale(options.zoom, options.zoom);
    pd.labelBackdrop.setScale(options.zoom, options.zoom);
}

// TODO: Fix -- doesn't work if zoomed out
void MassSysIsolatedDisplay::formLabelStacks(std::vector<PlanetDrawable>& planetDrawables, const sf::RenderWindow& window, DrawingOptions& options)
{
    std::sort(planetDrawables.begin(), planetDrawables.end());

    for (int i = 1; i < planetDrawables.size(); ++i)
    {
        PlanetDrawable& prevPD = planetDrawables[i - 1];
        PlanetDrawable& curPD = planetDrawables[i];

        int prevX = window.mapCoordsToPixel(prevPD.label.getPosition()).x;
        int curX = window.mapCoordsToPixel(curPD.label.getPosition()).x;

        int prevY = window.mapCoordsToPixel(prevPD.label.getPosition()).y;
        int curY = window.mapCoordsToPixel(curPD.label.getPosition()).y;

        sf::FloatRect prevBounds = prevPD.label.getGlobalBounds();
        sf::FloatRect curBounds = curPD.label.getGlobalBounds();

        if (curBounds.intersects(prevBounds))
        {
            curY = prevY + options.pxLabelYSpacing;
        }

        curPD.label.setPosition(window.mapPixelToCoords(sf::Vector2i(curX, curY)));
        curPD.labelBackdrop.setPosition(window.mapPixelToCoords(sf::Vector2i(curX, curY)));
    }
}

void MassSysIsolatedDisplay::drawAll(sf::RenderWindow& window, DrawingOptions& options)
{
    for (PlanetDrawable pd : this->getProcessedPlanetDrawables(window, options))
    {
        if (options.debug)
        {
            this->drawBounds(window, pd.circle);
            this->drawBounds(window, pd.label);
        }

        window.draw(pd.trail.data(), pd.trail.size(), sf::LinesStrip);
        window.draw(pd.circle);
        window.draw(pd.labelBackdrop);
        window.draw(pd.label);
    }
}

template<typename T>
void MassSysIsolatedDisplay::drawBounds(sf::RenderWindow& window, T& text)
{
    sf::Color colour;

    if (typeid(T) == typeid(sf::Text))
    {
        colour = sf::Color(210, 50, 235, 128);
    }
    else
    {
        colour = sf::Color(0, 255, 25, 128);
    }

    sf::RectangleShape globalBoundsRect;

    globalBoundsRect.setSize(sf::Vector2f(text.getGlobalBounds().width, text.getGlobalBounds().height));
    globalBoundsRect.setPosition(text.getPosition());

    globalBoundsRect.setOutlineColor(sf::Color(0, 0, 0, 0));
    globalBoundsRect.setFillColor(colour);

    window.draw(globalBoundsRect);
}