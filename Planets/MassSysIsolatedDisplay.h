#pragma once
#include "Display.h"
#include "MassSysIsolated.h"
#include <atomic>
#include <unordered_map>
#include <mutex>
#include <SFML/Graphics.hpp>

struct MBDrawable
{
    sf::CircleShape circle;
    sf::Text label;
    sf::RectangleShape labelBackdrop;
    std::vector<sf::Vertex> trail;

    bool operator <(const MBDrawable& pd) const
    {
        return (this->label.getPosition().y < pd.label.getPosition().y);
    }
};

struct HistoricalRenderData
{
    std::vector<sf::Vertex> trailVertices;
    unsigned int renderings;
};

class MassSysIsolatedDisplay : public Display
{
private:
    static const double SI_UNIT_MULTIPLIER;
    static const unsigned int TRAIL_VERTEX_COUNT_MAX;
    static const unsigned int RENDERINGS_PER_VERTEX;

    static sf::Color colourOf(MassiveBody& mb);

    void makeMBCircle(sf::CircleShape& circle, MassiveBody& mb, Vec2& pos);
    void makeMBTrail(sf::CircleShape& circle, MassiveBody& mb, Vec2& pos);
    void makeMBLabel(sf::Text& label, MassiveBody& mb, Vec2& pos);
    void makeMBLabelBackdrop(sf::RectangleShape& labelBackdrop, sf::Text& label, MassiveBody& mb, Vec2& pos);

    void applyZoom(MBDrawable& pd, DrawingOptions& options);
    void formLabelStacks(std::vector<MBDrawable>& planetDrawables, DrawingOptions& options);

    std::vector<MBDrawable> getUnprocessedPlanetDrawables();
    std::vector<MBDrawable> getProcessedPlanetDrawables(DrawingOptions& options);

public:
    MassSysIsolated& msi;
    std::unordered_map<unsigned int, HistoricalRenderData> mbRenderingData;

    explicit MassSysIsolatedDisplay(MassSysIsolated& msi, sf::RenderWindow& window, sf::View& view);
    explicit MassSysIsolatedDisplay(MassSysIsolated& msi, sf::RenderWindow& window, sf::View& view, sf::Font font);

    void calcPositionsAsync(std::atomic<bool>& running, std::mutex* mu);
    void drawAll(DrawingOptions& options) override;
};