#include "scenes/EndingScene.h"
#include "systems/GameSystem.h"
#include "levels.h"
#include "utils/SaveSystem.h"

void EndingScene::center(sf::Text& t, sf::Vector2f pos) {
    const auto b = t.getLocalBounds();
    t.setOrigin(b.left + b.width * 0.5f, b.top + b.height * 0.5f);
    t.setPosition(pos);
}

EndingScene::EndingScene() {
    (void)font.loadFromFile("resources/fonts/Kenney Future.ttf");

    fade.setSize({1280.f, 720.f});
    fade.setFillColor(sf::Color(15, 15, 25, 220));

    title.setFont(font);
    title.setCharacterSize(56);
    title.setFillColor(sf::Color(245, 245, 245));
    title.setString("REDEMPTION");

    body.setFont(font);
    body.setCharacterSize(22);
    body.setFillColor(sf::Color(230, 230, 230));
    body.setString(
        "You freed the souls bound to this place.\n"
        "The gates open.\n\n"
        "Your work is done.\n"
        "You may ascend."
    );

    hint.setFont(font);
    hint.setCharacterSize(18);
    hint.setFillColor(sf::Color(220, 220, 220));
    hint.setString("ENTER: Main Menu   |   R: Reset Save   |   ESC: Quit   |   M: Mute");

    GameSystem::playMusic("resources/music/menu.wav", true);
    GameSystem::applyMute();
}

void EndingScene::update(float dt) {
    (void)dt;
}

void EndingScene::render(sf::RenderWindow& window) {
    window.draw(fade);

    center(title, {640.f, 170.f});
    center(body,  {640.f, 360.f});
    center(hint,  {640.f, 650.f});

    window.draw(title);
    window.draw(body);
    window.draw(hint);
}

void EndingScene::handleEvent(sf::Event& event) {
    if (event.type != sf::Event::KeyPressed) return;

    if (event.key.code == sf::Keyboard::M) {
        Levels::muted = !Levels::muted;
        GameSystem::applyMute();
        SaveSystem::save(Levels::muted, Levels::level1Complete, Levels::level2Complete);
        return;
    }

    // Reset progress quickly (optional convenience)
    if (event.key.code == sf::Keyboard::R) {
        Levels::level1Complete = false;
        Levels::level2Complete = false;
        SaveSystem::save(Levels::muted, Levels::level1Complete, Levels::level2Complete);
        GameSystem::setActiveScene(Levels::menu);
        return;
    }

    if (event.key.code == sf::Keyboard::Enter ||
        event.key.code == sf::Keyboard::Return) {
        GameSystem::setActiveScene(Levels::menu);
        return;
    }

    if (event.key.code == sf::Keyboard::Escape) {
        GameSystem::quit();
        return;
    }
}
