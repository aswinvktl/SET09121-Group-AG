#pragma once
#include "scenes/Scene.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class MenuScene : public Scene {
public:
    MenuScene();
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;
    void handleEvent(sf::Event& event) override;

private:
    sf::Sprite grassTile;
    sf::Sprite stoneTile;

    sf::RectangleShape overlay;

    bool hasButtonTexture = false;
    sf::Sprite startButton;
    sf::Sprite quitButton;

    bool hasCursorTexture = false;
    sf::Sprite cursorSprite;

    sf::Font font;
    sf::Text title;
    sf::Text startLabel;
    sf::Text quitLabel;
    sf::Text hint;

    sf::SoundBuffer clickBuffer;
    sf::Sound clickSound;
    bool hasClick = false;

    static void centerText(sf::Text& t, sf::Vector2f pos);
    static bool hit(const sf::Sprite& s, sf::Vector2f p);
    void setHover(sf::Sprite& s, bool on);
    void playClick();
};
