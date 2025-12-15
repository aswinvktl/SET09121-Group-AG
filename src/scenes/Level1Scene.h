#pragma once
#include "scenes/Scene.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class Level1Scene : public Scene {
public:
    Level1Scene();

    void update(float dt) override;
    void render(sf::RenderWindow& window) override;
    void handleEvent(sf::Event& event) override;

    void reset(); // used by PauseScene

private:
    // World
    sf::Sprite floorTile;

    // Player
    sf::RectangleShape player;
    sf::Vector2f playerStart;
    float playerSpeed = 220.0f;
    sf::Sprite ghostSprite;

    // Door goal (replaces green goal)
    bool hasDoorClosed = false;
    bool hasDoorOpen = false;
    sf::Sprite doorSprite;
    bool doorOpened = false;
    float doorOpenTimer = 0.f;

    // Pools (water + label)
    struct Pool {
        sf::RectangleShape water;
        sf::Text label;
    };
    std::vector<Pool> pools;

    // Dog patrol
    struct Dog {
        sf::RectangleShape body;
        sf::Vector2f a;
        sf::Vector2f b;
        sf::Vector2f dir;
        float speed = 0.f;
        bool goingToB = true;
        float waitTimer = 0.f;
    };
    std::vector<Dog> dogs;

    // Optional sprites
    bool hasDogTexture = false;
    std::vector<sf::Sprite> dogSprites;

    // UI
    sf::Font font;
    sf::Text hud;

    // Audio
    sf::SoundBuffer clickBuffer;
    sf::Sound clickSound;
    bool hasClick = false;

    // Cached pointer to wall texture for pool borders (owned by Assets)
    const sf::Texture* wallTex = nullptr;

private:
    void playClick();
    void resetLevel();
    void win();

    void addPool(const sf::Vector2f& pos, const sf::Vector2f& size, const std::string& name);
    void addDog(const sf::Vector2f& a, const sf::Vector2f& b, float speed);

    void drawPoolBorder(sf::RenderWindow& window, const sf::RectangleShape& water) const;
};
