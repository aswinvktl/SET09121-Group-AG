#include "scenes/PauseScene.h"
#include "systems/GameSystem.h"
#include "levels.h"
#include "scenes/Level1Scene.h"

#include <memory>

void PauseScene::playClick() {
    if (hasClick && !Levels::muted) clickSound.play();
}

PauseScene::PauseScene() {
    overlay.setSize({1280.f, 720.f});
    overlay.setFillColor(sf::Color(0, 0, 0, 170));

    panel.setSize({720.f, 360.f});
    panel.setOrigin({360.f, 180.f});
    panel.setPosition({640.f, 360.f});
    panel.setFillColor(sf::Color(25, 25, 35, 230));
    panel.setOutlineThickness(4.f);
    panel.setOutlineColor(sf::Color(255, 255, 255, 25));

    (void)font.loadFromFile("resources/fonts/Kenney Future.ttf");

    title.setFont(font);
    title.setCharacterSize(42);
    title.setString("PAUSED");
    title.setPosition(540.f, 250.f);

    opts.setFont(font);
    opts.setCharacterSize(22);
    opts.setString("ESC/ENTER: Resume\nR: Restart level\nQ: Main menu\nM: Mute");
    opts.setPosition(520.f, 330.f);

    hasClick = clickBuffer.loadFromFile("resources/sfx/ui/click.ogg");
    if (hasClick) {
        clickSound.setBuffer(clickBuffer);
        clickSound.setVolume(60.f);
    }
}

void PauseScene::update(float dt) {
    (void)dt;
}

void PauseScene::render(sf::RenderWindow& window) {
    window.draw(overlay);
    window.draw(panel);
    window.draw(title);
    window.draw(opts);
}

void PauseScene::handleEvent(sf::Event& event) {
    if (event.type != sf::Event::KeyPressed) return;

    if (event.key.code == sf::Keyboard::M) {
        Levels::muted = !Levels::muted;
        return;
    }

    // Resume
    if (event.key.code == sf::Keyboard::Escape ||
        event.key.code == sf::Keyboard::Enter ||
        event.key.code == sf::Keyboard::Return) {
        playClick();
        if (Levels::pausedFrom) GameSystem::setActiveScene(Levels::pausedFrom);
        return;
    }

    // Restart (Level1 only, lab-simple)
    if (event.key.code == sf::Keyboard::R) {
        playClick();
        auto lvl1 = std::dynamic_pointer_cast<Level1Scene>(Levels::pausedFrom);
        if (lvl1) lvl1->reset();
        if (Levels::pausedFrom) GameSystem::setActiveScene(Levels::pausedFrom);
        return;
    }

    // Main menu
    if (event.key.code == sf::Keyboard::Q) {
        playClick();
        GameSystem::setActiveScene(Levels::menu);
        return;
    }
}
