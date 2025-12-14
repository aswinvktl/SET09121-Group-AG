#include "scenes/CemeteryScene.h"
#include "levels.h"
#include "systems/GameSystem.h"
#include <cmath>

sf::Vector2f CemeteryScene::normalizeOrZero(sf::Vector2f v) {
    const float len2 = v.x * v.x + v.y * v.y;
    if (len2 <= 0.0001f) return {0.f, 0.f};
    const float invLen = 1.0f / std::sqrt(len2);
    return {v.x * invLen, v.y * invLen};
}

CemeteryScene::CemeteryScene() {
    ghostRect.setSize(sf::Vector2f(40.f, 40.f));
    ghostRect.setFillColor(sf::Color(200, 200, 200, 255));
    ghostRect.setOrigin(20.f, 20.f);
    ghostPos = sf::Vector2f(640.f, 600.f);
    ghostRect.setPosition(ghostPos);

    grave1Rect.setSize(sf::Vector2f(60.f, 100.f));
    grave1Rect.setFillColor(sf::Color(80, 60, 40));
    grave1Rect.setOrigin(30.f, 50.f);
    grave1Rect.setPosition(640.f, 500.f);

    grave2Rect.setSize(sf::Vector2f(60.f, 100.f));
    grave2Rect.setFillColor(sf::Color(80, 60, 40));
    grave2Rect.setOrigin(30.f, 50.f);
    grave2Rect.setPosition(640.f, 350.f);

    grave3Rect.setSize(sf::Vector2f(60.f, 100.f));
    grave3Rect.setFillColor(sf::Color(80, 60, 40));
    grave3Rect.setOrigin(30.f, 50.f);
    grave3Rect.setPosition(640.f, 200.f);

    backgroundRect.setSize(sf::Vector2f(1280.f, 720.f));
    backgroundRect.setFillColor(sf::Color(20, 20, 40));
}

void CemeteryScene::update(float dt) {
    // Continuous, dt-based movement (normal + fluid)
    sf::Vector2f input(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) input.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) input.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) input.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) input.x += 1.f;

    sf::Vector2f dir = normalizeOrZero(input);
    ghostPos += dir * ghostSpeed * dt;

    // clamp to window bounds 
    const float half = 20.f; 
    if (ghostPos.x < half) ghostPos.x = half;
    if (ghostPos.x > 1280.f - half) ghostPos.x = 1280.f - half;
    if (ghostPos.y < half) ghostPos.y = half;
    if (ghostPos.y > 720.f - half) ghostPos.y = 720.f - half;

    ghostRect.setPosition(ghostPos);
}

void CemeteryScene::render(sf::RenderWindow& window) {
    // visual feedback for level 1 completion
    if (Levels::level1Complete) {
        grave1Rect.setFillColor(sf::Color(60, 160, 80));
    } else {
        grave1Rect.setFillColor(sf::Color(80, 60, 40));
    }

    window.draw(backgroundRect);
    window.draw(ghostRect);
    window.draw(grave1Rect);
    window.draw(grave2Rect);
    window.draw(grave3Rect);
}

void CemeteryScene::handleEvent(sf::Event& event) {
    // only handle interaction keys
    if (event.type != sf::Event::KeyPressed)
        return;

    if (event.key.code == sf::Keyboard::E) {
        if (isOverlapping(ghostRect, grave1Rect)) {
            GameSystem::setActiveScene(Levels::level1);
        } else if (isOverlapping(ghostRect, grave2Rect)) {
            GameSystem::setActiveScene(Levels::level2);
        }
    }
}

bool CemeteryScene::isOverlapping(const sf::RectangleShape& a,
                                  const sf::RectangleShape& b) {
    return a.getGlobalBounds().intersects(b.getGlobalBounds());
}
