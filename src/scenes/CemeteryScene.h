#pragma once

#include "scenes/Scene.h"
#include <SFML/Graphics.hpp>
#include <vector>

class CemeteryScene : public Scene {
public:
    CemeteryScene();

    void update(float dt) override;
    void render(sf::RenderWindow& window) override;
    void handleEvent(sf::Event& event) override;

private:
    // tiles
    sf::Sprite grassTile;
    sf::Sprite stoneTile;

    // player (ghost)
    sf::Sprite ghostSprite;
    sf::RectangleShape ghostHitbox;
    sf::Vector2f ghostPos;
    float ghostSpeed = 280.0f;

    // graves (rects are interaction/hit boxes only; not rendered)
    sf::RectangleShape grave1Rect;
    sf::RectangleShape grave2Rect;
    sf::RectangleShape grave3Rect;

    // grave visuals
    bool hasGraveTexture = false;
    bool hasFlowerTexture = false;
    sf::Sprite grave1Sprite, grave2Sprite, grave3Sprite;

    // labels under graves
    sf::Font font;
    sf::Text banner;
    sf::Text interact;
    sf::Text level1Label, level2Label, level3Label;

    // blockers
    std::vector<sf::RectangleShape> blockers;

    static sf::Vector2f normalizeOrZero(sf::Vector2f v);
    static bool overlaps(const sf::RectangleShape& a, const sf::RectangleShape& b);

    void setupLevelLabel(sf::Text& t, const sf::String& s, sf::Vector2f pos);
};
