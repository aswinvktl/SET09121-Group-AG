#include "scenes/CemeteryScene.h"
#include "levels.h"
#include "systems/GameSystem.h"
#include "assets/Assets.h"
#include <cmath>

sf::Vector2f CemeteryScene::normalizeOrZero(sf::Vector2f v) {
    float len2 = v.x * v.x + v.y * v.y;
    if (len2 <= 0.0001f) return {0.f, 0.f};
    float inv = 1.0f / std::sqrt(len2);
    return {v.x * inv, v.y * inv};
}

CemeteryScene::CemeteryScene() {
    // background
    backgroundRect.setSize({1280.f, 720.f});
    backgroundRect.setFillColor(sf::Color(20, 20, 40));

    // ghost position
    ghostPos = {640.f, 600.f};

    // ghost hitbox
    ghostHitbox.setSize({64.f, 64.f});
    ghostHitbox.setOrigin(32.f, 32.f);
    ghostHitbox.setPosition(ghostPos);
    ghostHitbox.setFillColor(sf::Color(255, 0, 0, 0)); // invisible

    // ghost sprite 
    ghostSprite.setTexture(Assets::getTexture("ghost"));
    ghostSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
    ghostSprite.setOrigin(16.f, 16.f);
    ghostSprite.setScale(2.f, 2.f);
    ghostSprite.setPosition(ghostPos);

    // graves
    grave1Rect.setSize({60.f, 100.f});
    grave1Rect.setOrigin(30.f, 50.f);
    grave1Rect.setPosition(640.f, 500.f);
    grave1Rect.setFillColor(sf::Color(80, 60, 40));

    grave2Rect.setSize({60.f, 100.f});
    grave2Rect.setOrigin(30.f, 50.f);
    grave2Rect.setPosition(640.f, 350.f);
    grave2Rect.setFillColor(sf::Color(80, 60, 40));

    grave3Rect.setSize({60.f, 100.f});
    grave3Rect.setOrigin(30.f, 50.f);
    grave3Rect.setPosition(640.f, 200.f);
    grave3Rect.setFillColor(sf::Color(80, 60, 40));
}

void CemeteryScene::update(float dt) {
    sf::Vector2f input(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) input.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) input.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) input.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) input.x += 1.f;

    ghostPos += normalizeOrZero(input) * ghostSpeed * dt;

    // clamp to window bounds
    if (ghostPos.x < 32.f) ghostPos.x = 32.f;
    if (ghostPos.x > 1280.f - 32.f) ghostPos.x = 1280.f - 32.f;
    if (ghostPos.y < 32.f) ghostPos.y = 32.f;
    if (ghostPos.y > 720.f - 32.f) ghostPos.y = 720.f - 32.f;

    ghostSprite.setPosition(ghostPos);
    ghostHitbox.setPosition(ghostPos);
}

void CemeteryScene::render(sf::RenderWindow& window) {
    //progress feedback
    if (Levels::level1Complete)
        grave1Rect.setFillColor(sf::Color(60, 160, 80));
    else
        grave1Rect.setFillColor(sf::Color(80, 60, 40));

    window.draw(backgroundRect);
    window.draw(ghostSprite);
    window.draw(grave1Rect);
    window.draw(grave2Rect);
    window.draw(grave3Rect);
}

void CemeteryScene::handleEvent(sf::Event& event) {
    if (event.type != sf::Event::KeyPressed) return;

    if (event.key.code == sf::Keyboard::E) {
        if (isOverlapping(ghostHitbox, grave1Rect)) {
            GameSystem::setActiveScene(Levels::level1);
        } else if (isOverlapping(ghostHitbox, grave2Rect)) {
            GameSystem::setActiveScene(Levels::level2);
        }
    }
}

bool CemeteryScene::isOverlapping(const sf::RectangleShape& a,
                                  const sf::RectangleShape& b) {
    return a.getGlobalBounds().intersects(b.getGlobalBounds());
}
