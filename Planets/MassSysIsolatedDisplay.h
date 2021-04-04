#pragma once
#include "MassSysIsolated.h"
#include <atomic>
#include <unordered_map>
#include <mutex>
#include <SFML/Graphics.hpp>

struct PlanetDrawable
{
    sf::CircleShape circle;
    sf::Text label;
    sf::RectangleShape labelBackdrop;
    std::vector<sf::Vertex> trail;

    bool operator <(const PlanetDrawable& pd) const
    {
        return (this->label.getPosition().y < pd.label.getPosition().y);
    }
};

struct DrawingOptions
{
    int pxLabelYSpacing;
    float zoom;
    float scalingThreshold;
    bool debug;
};

class MassSysIsolatedDisplay
{
private:
    static const double LENGTH_SCALE_DOWN_FACTOR;
    static const unsigned int TRAIL_SIZE_MAX;

    static sf::Font font;

    static sf::Color colourOf(MassiveBody& mb);

    void initCircle(sf::CircleShape& circle, MassiveBody& mb, Vec2& pos);
    void initTrail(sf::CircleShape& circle, MassiveBody& mb, Vec2& pos);
    void initLabel(sf::Text& label, MassiveBody& mb, Vec2& pos);
    void initLabelBackdrop(sf::RectangleShape& labelBackdrop, sf::Text& label, MassiveBody& mb, Vec2& pos);

    void applyZoom(PlanetDrawable& pd, DrawingOptions& options);
    void formLabelStacks(std::vector<PlanetDrawable>& planetDrawables, const sf::RenderWindow& window, DrawingOptions& options);

    std::vector<PlanetDrawable> getUnprocessedPlanetDrawables();
    std::vector<PlanetDrawable> getProcessedPlanetDrawables(const sf::RenderWindow& window, DrawingOptions& options);

    template<typename T>
    void drawBounds(sf::RenderWindow& window, T& text);

public:
    MassSysIsolated& msi;
    std::unordered_map<unsigned int, std::vector<sf::Vertex>> trails;

    MassSysIsolatedDisplay(MassSysIsolated& msi);

    void calcPositions(std::atomic<bool>& running, std::mutex* mu);
    void drawAll(sf::RenderWindow& window, DrawingOptions& options);
};