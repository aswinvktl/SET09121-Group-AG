#include "scenes/CemeteryScene.h"

#include "assets/Assets.h"
#include "levels.h"
#include "systems/GameSystem.h"

#include <algorithm>
#include <cmath>

sf::Vector2f CemeteryScene::normalizeOrZero(sf::Vector2f v) {
    const float len2 = v.x * v.x + v.y * v.y;
    if (len2 <= 0.0001f) return {0.f, 0.f};
    const float inv = 1.0f / std::sqrt(len2);
    return {v.x * inv, v.y * inv};
}

bool CemeteryScene::isOverlapping(const sf::RectangleShape& a, const sf::RectangleShape& b) {
    return a.getGlobalBounds().intersects(b.getGlobalBounds());
}

CemeteryScene::CemeteryScene() {
    grassTile.setTexture(Assets::getTexture("tiles_grass"));
    grassTile.setTextureRect({0, 0, 32, 32});
    grassTile.setScale(2.f, 2.f);

    ghostPos = {640.f, 600.f};

    ghostHitbox.setSize({64.f, 64.f});
    ghostHitbox.setOrigin(32.f, 32.f);
    ghostHitbox.setPosition(ghostPos);
    ghostHitbox.setFillColor(sf::Color(255, 0, 0, 0));

    ghostSprite.setTexture(Assets::getTexture("ghost"));
    ghostSprite.setTextureRect({0, 0, 32, 32});
    ghostSprite.setOrigin(16.f, 16.f);
    ghostSprite.setScale(2.f, 2.f);
    ghostSprite.setPosition(ghostPos);

    auto setupGrave = [](sf::RectangleShape& r, sf::Vector2f pos) {
        r.setSize({60.f, 100.f});
        r.setOrigin(30.f, 50.f);
        r.setPosition(pos);
        r.setFillColor(sf::Color(80, 60, 40));
    };
    setupGrave(grave1Rect, {640.f, 500.f});
    setupGrave(grave2Rect, {640.f, 350.f});
    setupGrave(grave3Rect, {640.f, 200.f});

    hasProps = Assets::hasTexture("props");
    if (hasProps) {
        grave1Sprite.setTexture(Assets::getTexture("props"));
        grave2Sprite.setTexture(Assets::getTexture("props"));
        grave3Sprite.setTexture(Assets::getTexture("props"));

        grave1Sprite.setTextureRect(graveRect);
        grave2Sprite.setTextureRect(graveRect);
        grave3Sprite.setTextureRect(graveRect);

        auto setupSprite = [](sf::Sprite& s, sf::Vector2f pos) {
            s.setOrigin(16.f, 16.f);
            s.setScale(2.f, 2.f);
            s.setPosition(pos);
        };
        setupSprite(grave1Sprite, grave1Rect.getPosition());
        setupSprite(grave2Sprite, grave2Rect.getPosition());
        setupSprite(grave3Sprite, grave3Rect.getPosition());
    }

    (void)font.loadFromFile("resources/fonts/Kenney Future.ttf");
    prompt.setFont(font);
    prompt.setCharacterSize(20);
    prompt.setFillColor(sf::Color(230, 230, 230));
    prompt.setString("Press E at a grave to enter a level");
    prompt.setPosition(20.f, 20.f);
}

void CemeteryScene::update(float dt) {
    sf::Vector2f input(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) input.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) input.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) input.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) input.x += 1.f;

    ghostPos += normalizeOrZero(input) * ghostSpeed * dt;

    ghostPos.x = std::clamp(ghostPos.x, 32.f, 1280.f - 32.f);
    ghostPos.y = std::clamp(ghostPos.y, 32.f, 720.f - 32.f);

    ghostSprite.setPosition(ghostPos);
    ghostHitbox.setPosition(ghostPos);
}

void CemeteryScene::render(sf::RenderWindow& window) {
    for (int y = 0; y < 12; ++y) {
        for (int x = 0; x < 20; ++x) {
            grassTile.setPosition(x * 64.f, y * 64.f);
            window.draw(grassTile);
        }
    }

    grave1Rect.setFillColor(Levels::level1Complete ? sf::Color(60, 160, 80) : sf::Color(80, 60, 40));
    grave3Sprite.setColor(Levels::level3Complete ? sf::Color(160, 255, 160) : sf::Color::White);

    if (hasProps) {
        grave1Sprite.setColor(Levels::level1Complete ? sf::Color(160, 255, 160) : sf::Color::White);
        window.draw(grave1Sprite);
        window.draw(grave2Sprite);
        window.draw(grave3Sprite);
    } else {
        window.draw(grave1Rect);
        window.draw(grave2Rect);
        window.draw(grave3Rect);
    }

    window.draw(ghostSprite);
    window.draw(prompt);
}

void CemeteryScene::handleEvent(sf::Event& event) {
    if (event.type != sf::Event::KeyPressed) return;

    if (event.key.code == sf::Keyboard::E) {
        if (isOverlapping(ghostHitbox, grave1Rect)) {
            GameSystem::setActiveScene(Levels::level1);
        }
        else if (isOverlapping(ghostHitbox, grave2Rect)) {
            GameSystem::setActiveScene(Levels::level2);
        }
        else if (isOverlapping(ghostHitbox, grave3Rect)) {
            GameSystem::setActiveScene(Levels::level3);
        }
    }
    else if (event.key.code == sf::Keyboard::Escape) {
        Levels::pausedFrom = Levels::cemetery;
        GameSystem::setActiveScene(Levels::pause);
    }
}