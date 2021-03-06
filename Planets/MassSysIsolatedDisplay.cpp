#include "MassSysIsolatedDisplay.h"
#include <algorithm>
#include <future>
#include <Windows.h>
#include <iostream>

const double MassSysIsolatedDisplay::SI_UNIT_MULTIPLIER = 1.0e-6;
const unsigned int MassSysIsolatedDisplay::TRAIL_VERTEX_COUNT_MAX = (unsigned int)1.0e4;
const unsigned int MassSysIsolatedDisplay::RENDERINGS_PER_VERTEX = 8;

MassSysIsolatedDisplay::MassSysIsolatedDisplay(MassSysIsolated& msi, sf::RenderWindow& window, sf::View& view) : msi(msi), Display(window, view)
{
    this->font.loadFromFile("fonts/Ubuntu/Ubuntu-Medium.ttf");
}

MassSysIsolatedDisplay::MassSysIsolatedDisplay(MassSysIsolated& msi, sf::RenderWindow& window, sf::View& view, sf::Font font) : msi(msi), Display(window, view, font)
{
    this->font.loadFromFile("fonts/Ubuntu/Ubuntu-Medium.ttf");
}

void MassSysIsolatedDisplay::calcPositionsAsync(std::atomic<bool>& running, std::mutex* mu)
{
    while (running.load())
    {
        mu->lock();
        this->msi.budgeAll(0.1);
        std::cout << "USING DEFAULT DT" << std::endl;
        mu->unlock();
    }
}

std::vector<MBDrawable> MassSysIsolatedDisplay::getUnprocessedPlanetDrawables()
{
    std::vector<MBDrawable> planets = {};

    for (MassiveBody& mb : this->msi.mbs)
    {
        HistoricalRenderData& rDataCur = this->mbRenderingData[mb.getID()];
        Vec2 pos = mb.getPos(MassSysIsolatedDisplay::SI_UNIT_MULTIPLIER);

        sf::CircleShape circle;
        this->makeMBCircle(circle, mb, pos);

        if (rDataCur.renderings % MassSysIsolatedDisplay::RENDERINGS_PER_VERTEX == 0)
        {
            this->makeMBTrail(circle, mb, pos);
        }

        sf::Text label;
        this->makeMBLabel(label, mb, pos);

        planets.emplace_back(MBDrawable{ circle, label, &rDataCur.trailVertices });
        ++rDataCur.renderings;
    }

    return planets;
}

sf::Color MassSysIsolatedDisplay::colourOf(MassiveBody& mb)
{
    Colour _c = SOLAR_SYS_SI_COLOURS.at(mb.type);
    return sf::Color(_c.r, _c.g, _c.b);
}

void MassSysIsolatedDisplay::makeMBCircle(sf::CircleShape& circle, MassiveBody& mb, Vec2& pos)
{
    circle.setRadius(mb.getRadius(MassSysIsolatedDisplay::SI_UNIT_MULTIPLIER));
    circle.setPosition(pos.x, pos.y);
    circle.setFillColor(MassSysIsolatedDisplay::colourOf(mb));
}

void MassSysIsolatedDisplay::makeMBTrail(sf::CircleShape& circle, MassiveBody& mb, Vec2& pos)
{
    unsigned int id = mb.getID();

    if (this->mbRenderingData.find(id) == this->mbRenderingData.end())
    {
        this->mbRenderingData[id] = { std::vector<sf::Vertex>(), 0 };
    }

    sf::Vector2f trailPoint(pos.x + circle.getRadius(), pos.y + circle.getRadius());

    sf::Color colour = MassSysIsolatedDisplay::colourOf(mb);

    std::vector<sf::Vertex>& trailVertices = this->mbRenderingData[id].trailVertices;
    trailVertices.emplace_back(sf::Vertex(trailPoint, colour));

    if (trailVertices.size() > MassSysIsolatedDisplay::TRAIL_VERTEX_COUNT_MAX)
    {
        trailVertices.erase(trailVertices.begin(), trailVertices.begin() + (trailVertices.size() - MassSysIsolatedDisplay::TRAIL_VERTEX_COUNT_MAX));
    }
    // std::cout << trailVertices.size() << std::endl;
}

void MassSysIsolatedDisplay::makeMBLabel(sf::Text& label, MassiveBody& mb, Vec2& pos)
{
    label.setFont(this->font);
    label.setString(mb.getName());
    label.setCharacterSize(14);
    //label.setFillColor(sf::Color::Black);
    label.setFillColor(sf::Color::White);
    label.setOutlineColor(sf::Color::Black);
    label.setOutlineThickness(2);
    label.setPosition(pos.x, pos.y);
}

std::vector<MBDrawable> MassSysIsolatedDisplay::getProcessedPlanetDrawables(DrawingOptions& options)
{
    std::vector<MBDrawable> planetDrawables = this->getUnprocessedPlanetDrawables();

    for (MBDrawable& pd : planetDrawables)
    {
        this->applyZoom(pd, options);
    }

    this->formLabelStacks(planetDrawables, options);

    auto compSize = [](MBDrawable& a, MBDrawable& b) {
        return (a.circle.getRadius() > b.circle.getRadius());
    };

    std::sort(planetDrawables.begin(), planetDrawables.end(), compSize);

    return planetDrawables;
}

void MassSysIsolatedDisplay::applyZoom(MBDrawable& pd, DrawingOptions& options)
{
    pd.label.setScale(options.zoom, options.zoom);
}

void MassSysIsolatedDisplay::formLabelStacks(std::vector<MBDrawable>& planetDrawables, DrawingOptions& options)
{
    auto compYPos = [](MBDrawable& a, MBDrawable& b) {
        return (a.label.getPosition().y < b.label.getPosition().y);
    };

    std::sort(planetDrawables.begin(), planetDrawables.end(), compYPos);

    for (unsigned int i = 1; i < planetDrawables.size(); ++i)
    {
        MBDrawable& prevPD = planetDrawables[i - 1];
        MBDrawable& curPD = planetDrawables[i];

        int prevX = this->window.mapCoordsToPixel(prevPD.label.getPosition()).x;
        int curX = this->window.mapCoordsToPixel(curPD.label.getPosition()).x;

        int prevY = this->window.mapCoordsToPixel(prevPD.label.getPosition()).y;
        int curY = this->window.mapCoordsToPixel(curPD.label.getPosition()).y;

        sf::FloatRect prevBounds = prevPD.label.getGlobalBounds();
        sf::FloatRect curBounds = curPD.label.getGlobalBounds();

        if (curBounds.intersects(prevBounds))
        {
            curY = prevY + options.pxLabelYSpacing;
        }

        curPD.label.setPosition(this->window.mapPixelToCoords(sf::Vector2i(curX, curY)));
    }
}

void MassSysIsolatedDisplay::drawAll(DrawingOptions& options)
{
    sf::View oldView = this->window.getView();
    this->window.setView(this->view);

    const std::vector<MBDrawable> pds = this->getProcessedPlanetDrawables(options);

    for (const MBDrawable& pd : pds) { window.draw(pd.trail->data(), pd.trail->size(), sf::LinesStrip); }
    for (const MBDrawable& pd : pds)
    {
        if (options.debug) this->drawBoundsFor(pd.circle);
        window.draw(pd.circle);
    }
    for (const MBDrawable& pd : pds)
    {
        if (options.debug) this->drawBoundsFor(pd.label);

        window.draw(pd.label);
    }

    this->window.setView(oldView);
}
