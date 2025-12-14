#include "scenes/PauseScene.h"
#include "systems/GameSystem.h"
#include "levels.h"

PauseScene::PauseScene() {
    // Dark transparent overlay
    overlay.setSize({1280.f, 720.f});
    overlay.setFillColor(sf::Color(0, 0, 0, 150));

    // Central pause panel
    panel.setSize({600.f, 300.f});
    panel.setOrigin({300.f, 150.f});
    panel.setPosition({640.f, 360.f});
    panel.setFillColor(sf::Color(40, 40, 70));
}

void PauseScene::update(float dt) {
    (void)dt;
}

void PauseScene::render(sf::RenderWindow& window) {
    // IMPORTANT: no clear() here
    window.draw(overlay);
    window.draw(panel);
}

void PauseScene::handleEvent(sf::Event& event) {
    if (event.type != sf::Event::KeyPressed)
        return;

    // Resume (Escape or Enter)
    if (event.key.code == sf::Keyboard::Escape ||
        event.key.code == sf::Keyboard::Enter ||
        event.key.code == sf::Keyboard::Return) {

        if (Levels::pausedFrom) {
            GameSystem::setActiveScene(Levels::pausedFrom);
        }
        return;
    }

    // Quit to main menu
    if (event.key.code == sf::Keyboard::Q) {
        GameSystem::setActiveScene(Levels::menu);
        return;
    }
}
