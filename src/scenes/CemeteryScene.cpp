#include "scenes/CemeteryScene.h"
#include "levels.h"
#include "systems/GameSystem.h"

// setup ghost and 3 graves
CemeteryScene::CemeteryScene() {
    ghostRect.setSize(sf::Vector2f(40.f, 40.f));
    ghostRect.setFillColor(sf::Color(200, 200, 200, 255));
    ghostRect.setOrigin(20.f, 20.f);
    ghostPos = sf::Vector2f(640.f, 600.f);

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
    ghostRect.setPosition(ghostPos);
}

void CemeteryScene::render(sf::RenderWindow& window) {
    window.draw(backgroundRect);
    window.draw(ghostRect);
    window.draw(grave1Rect);
    window.draw(grave2Rect);
    window.draw(grave3Rect);
}

void CemeteryScene::handleEvent(sf::Event& event) {
    if (event.type != sf::Event::KeyPressed)
        return;

    if (event.key.code == sf::Keyboard::W) ghostPos.y -= ghostSpeed;
    if (event.key.code == sf::Keyboard::S) ghostPos.y += ghostSpeed;
    if (event.key.code == sf::Keyboard::A) ghostPos.x -= ghostSpeed;
    if (event.key.code == sf::Keyboard::D) ghostPos.x += ghostSpeed;

    if (event.key.code == sf::Keyboard::E) {
        if (isOverlapping(ghostRect, grave1Rect)) {
            // go to Level 1
            GameSystem::setActiveScene(Levels::level1);
        } else if (isOverlapping(ghostRect, grave2Rect)) {
            // go to Level 2
            GameSystem::setActiveScene(Levels::level2);
        }
    }
}

bool CemeteryScene::isOverlapping(const sf::RectangleShape& a,
                                  const sf::RectangleShape& b) {
    return a.getGlobalBounds().intersects(b.getGlobalBounds());
}
