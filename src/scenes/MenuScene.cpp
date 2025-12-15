#include "scenes/MenuScene.h"
#include "assets/Assets.h"
#include "levels.h"
#include "systems/GameSystem.h"

static float clampf(float v, float lo, float hi) {
    return (v < lo) ? lo : (v > hi) ? hi : v;
}

void MenuScene::centerText(sf::Text& t, sf::Vector2f pos) {
    const auto b = t.getLocalBounds();
    t.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
    t.setPosition(pos);
}

bool MenuScene::hit(const sf::Sprite& s, sf::Vector2f p) {
    return s.getGlobalBounds().contains(p);
}

void MenuScene::setHover(sf::Sprite& s, bool on) {
    s.setScale(on ? 2.75f : 2.6f, on ? 2.75f : 2.6f);
}

void MenuScene::playClick() {
    if (hasClick && !Levels::muted) clickSound.play();
}

MenuScene::MenuScene() {
    grassTile.setTexture(Assets::getTexture("grass"));
    grassTile.setTextureRect({0, 0, 32, 32});
    grassTile.setScale(2.f, 2.f);

    stoneTile.setTexture(Assets::getTexture("stone"));
    stoneTile.setTextureRect({0, 0, 32, 32});
    stoneTile.setScale(2.f, 2.f);

    overlay.setSize({1280.f, 720.f});
    overlay.setFillColor(sf::Color(0, 0, 0, 170));

    hasButtonTexture = Assets::hasTexture("ui_button");
    if (hasButtonTexture) {
        startButton.setTexture(Assets::getTexture("ui_button"));
        quitButton.setTexture(Assets::getTexture("ui_button"));

        startButton.setColor(sf::Color(170, 170, 170));
        quitButton.setColor(sf::Color(170, 170, 170));

        auto setupBtn = [](sf::Sprite& s) {
            const auto b = s.getLocalBounds();
            s.setOrigin(b.width / 2.f, b.height / 2.f);
            s.setScale(2.6f, 2.6f);
        };
        setupBtn(startButton);
        setupBtn(quitButton);
    }

    hasCursorTexture = Assets::hasTexture("ui_cursor");
    if (hasCursorTexture) {
        cursorSprite.setTexture(Assets::getTexture("ui_cursor"));
        cursorSprite.setScale(2.f, 2.f);
        GameSystem::window().setMouseCursorVisible(false);
    } else {
        GameSystem::window().setMouseCursorVisible(true);
    }

    hasClick = clickBuffer.loadFromFile("resources/sfx/ui/click.ogg");
    if (hasClick) {
        clickSound.setBuffer(clickBuffer);
        clickSound.setVolume(60.f);
    }

    (void)font.loadFromFile("resources/fonts/Kenney Future.ttf");

    title.setFont(font);
    title.setString("GHOST REDEMPTION");
    title.setCharacterSize(56);
    title.setFillColor(sf::Color(245, 245, 245));

    startLabel.setFont(font);
    startLabel.setString("START");
    startLabel.setCharacterSize(32);
    startLabel.setFillColor(sf::Color(245, 245, 245));

    quitLabel.setFont(font);
    quitLabel.setString("QUIT");
    quitLabel.setCharacterSize(32);
    quitLabel.setFillColor(sf::Color(245, 245, 245));

    hint.setFont(font);
    hint.setString("ENTER: START   |   ESC: QUIT   |   M: MUTE");
    hint.setCharacterSize(20);
    hint.setFillColor(sf::Color(230, 230, 230));
}

void MenuScene::update(float) {
    auto& win = GameSystem::window();
    const sf::Vector2f mouse =
        win.mapPixelToCoords(sf::Mouse::getPosition(win), win.getView());

    if (hasCursorTexture) cursorSprite.setPosition(mouse);

    if (hasButtonTexture) {
        setHover(startButton, hit(startButton, mouse));
        setHover(quitButton,  hit(quitButton, mouse));
    }
}

void MenuScene::render(sf::RenderWindow& window) {
    // Background (grass only, no random stone band)
    for (int y = 0; y < 12; ++y) {
        for (int x = 0; x < 20; ++x) {
            grassTile.setPosition(x * 64.f, y * 64.f);
            window.draw(grassTile);
        }
    }

    window.draw(overlay);

    // Layout (fixed virtual space 1280x720)
    const float cx = 640.f;

    // Title position
    centerText(title, {cx, 170.f});

    // Buttons: centered, separated, not overlapping
    if (hasButtonTexture) {
        startButton.setPosition({cx, 340.f});
        quitButton.setPosition({cx, 460.f});
        centerText(startLabel, {cx, 340.f});
        centerText(quitLabel,  {cx, 460.f});
    } else {
        // fallback text-only layout
        centerText(startLabel, {cx, 340.f});
        centerText(quitLabel,  {cx, 460.f});
    }

    // Hint at bottom
    centerText(hint, {cx, 660.f});

    // Draw
    if (hasButtonTexture) {
        window.draw(startButton);
        window.draw(quitButton);
    }

    window.draw(title);
    window.draw(startLabel);
    window.draw(quitLabel);
    window.draw(hint);

    if (hasCursorTexture) window.draw(cursorSprite);
}

void MenuScene::handleEvent(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::M) {
            Levels::muted = !Levels::muted;
            return;
        }
        if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Return) {
            playClick();
            GameSystem::setActiveScene(Levels::cemetery);
            return;
        }
        if (event.key.code == sf::Keyboard::Escape) {
            playClick();
            GameSystem::quit();
            return;
        }
    }

    if (!hasButtonTexture) return;

    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {

        auto& win = GameSystem::window();
        const sf::Vector2f mouse =
            win.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y}, win.getView());

        if (hit(startButton, mouse)) {
            playClick();
            GameSystem::setActiveScene(Levels::cemetery);
        } else if (hit(quitButton, mouse)) {
            playClick();
            GameSystem::quit();
        }
    }
}
