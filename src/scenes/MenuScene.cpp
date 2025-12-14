#include "scenes/MenuScene.h"
#include "systems/GameSystem.h"
#include "levels.h"

MenuScene::MenuScene() {
    background.setSize({1280.f, 720.f});
    background.setFillColor(sf::Color(10, 10, 20));

    titleBar.setSize({900.f, 120.f});
    titleBar.setOrigin({450.f, 60.f});
    titleBar.setPosition({640.f, 200.f});
    titleBar.setFillColor(sf::Color(30, 30, 60));

    startButton.setSize({420.f, 90.f});
    startButton.setOrigin({210.f, 45.f});
    startButton.setPosition({640.f, 420.f});
    startButton.setFillColor(sf::Color(60, 160, 80));
}

void MenuScene::update(float dt) {
    (void)dt; // nothing to update
}

void MenuScene::render(sf::RenderWindow& window) {
    window.draw(background);
    window.draw(titleBar);
    window.draw(startButton);
}

void MenuScene::handleEvent(sf::Event& event) {
    if (event.type != sf::Event::KeyPressed) return;

    // Press Enter to start
    if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Return) {
        GameSystem::setActiveScene(Levels::cemetery);
    }
}
