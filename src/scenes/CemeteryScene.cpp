#include "scenes/CemeteryScene.h"

#include "assets/Assets.h"
#include "levels.h"
#include "systems/GameSystem.h"

#include <cmath>

static float clampf(float v, float lo, float hi) {
    return (v < lo) ? lo : (v > hi) ? hi : v;
}

sf::Vector2f CemeteryScene::normalizeOrZero(sf::Vector2f v) {
    const float len2 = v.x * v.x + v.y * v.y;
    if (len2 <= 0.0001f) return {0.f, 0.f};
    const float inv = 1.0f / std::sqrt(len2);
    return {v.x * inv, v.y * inv};
}

bool CemeteryScene::overlaps(const sf::RectangleShape& a, const sf::RectangleShape& b) {
    return a.getGlobalBounds().intersects(b.getGlobalBounds());
}

void CemeteryScene::setupLevelLabel(sf::Text& t, const sf::String& s, sf::Vector2f pos) {
    t.setFont(font);
    t.setString(s);
    t.setCharacterSize(18);
    t.setFillColor(sf::Color(240, 240, 240));
    t.setOutlineThickness(2.f);
    t.setOutlineColor(sf::Color(0, 0, 0, 150));

    const auto b = t.getLocalBounds();
    t.setOrigin(b.left + b.width * 0.5f, b.top + b.height * 0.5f);
    t.setPosition(pos);
}

CemeteryScene::CemeteryScene() {
    grassTile.setTexture(Assets::getTexture("grass"));
    grassTile.setTextureRect({0, 0, 32, 32});
    grassTile.setScale(2.f, 2.f);

    stoneTile.setTexture(Assets::getTexture("stone"));
    stoneTile.setTextureRect({0, 0, 32, 32});
    stoneTile.setScale(2.f, 2.f);

    ghostPos = {640.f, 620.f};

    ghostHitbox.setSize({46.f, 46.f});
    ghostHitbox.setOrigin(23.f, 23.f);
    ghostHitbox.setPosition(ghostPos);
    ghostHitbox.setFillColor(sf::Color(255, 0, 0, 0));

    ghostSprite.setTexture(Assets::getTexture("ghost"));
    ghostSprite.setTextureRect({0, 0, 32, 32});
    ghostSprite.setOrigin(16.f, 16.f);
    ghostSprite.setScale(2.f, 2.f);
    ghostSprite.setPosition(ghostPos);

    auto setupGrave = [](sf::RectangleShape& r, sf::Vector2f pos) {
        r.setSize({60.f, 92.f});
        r.setOrigin(30.f, 46.f);
        r.setPosition(pos);
    };

    setupGrave(grave1Rect, {640.f, 500.f});
    setupGrave(grave2Rect, {640.f, 350.f});
    setupGrave(grave3Rect, {640.f, 200.f});

    // only side blockers
    auto addBlock = [&](sf::Vector2f pos, sf::Vector2f size) {
        sf::RectangleShape r;
        r.setPosition(pos);
        r.setSize(size);
        r.setFillColor(sf::Color(255, 0, 0, 0));
        blockers.push_back(r);
    };
    addBlock({120.f, 80.f},  {40.f, 560.f});
    addBlock({1120.f, 80.f}, {40.f, 560.f});

    (void)font.loadFromFile("resources/fonts/Kenney Future.ttf");

    banner.setFont(font);
    banner.setCharacterSize(20);
    banner.setFillColor(sf::Color(230, 230, 230));
    banner.setString("WASD/Arrows: move   |   E: enter level   |   ESC: pause   |   M: mute");
    banner.setPosition(20.f, 20.f);

    interact.setFont(font);
    interact.setCharacterSize(22);
    interact.setFillColor(sf::Color(255, 255, 255));
    interact.setString("Press E");

    setupLevelLabel(level1Label, "LEVEL 1", {grave1Rect.getPosition().x, grave1Rect.getPosition().y + 70.f});
    setupLevelLabel(level2Label, "LEVEL 2", {grave2Rect.getPosition().x, grave2Rect.getPosition().y + 70.f});
    setupLevelLabel(level3Label, "LEVEL 3", {grave3Rect.getPosition().x, grave3Rect.getPosition().y + 70.f});

    hasGraveTexture = Assets::hasTexture("grave");
    hasFlowerTexture = Assets::hasTexture("flowerbed");

    if (hasGraveTexture) {
        grave1Sprite.setTexture(Assets::getTexture("grave"));
        grave2Sprite.setTexture(Assets::getTexture("grave"));
        grave3Sprite.setTexture(Assets::getTexture("grave"));

        auto setupSprite = [](sf::Sprite& s, sf::Vector2f pos) {
            const auto b = s.getLocalBounds();
            s.setOrigin(b.width * 0.5f, b.height * 0.5f);
            s.setScale(2.f, 2.f);
            s.setPosition(pos);
        };
        setupSprite(grave1Sprite, grave1Rect.getPosition());
        setupSprite(grave2Sprite, grave2Rect.getPosition());
        setupSprite(grave3Sprite, grave3Rect.getPosition());
    }

    // MUSIC (global gameplay loop)
    GameSystem::playMusic("resources/music/gameplay.wav", true);
}

void CemeteryScene::update(float dt) {
    sf::Vector2f input(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) input.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) input.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) input.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) input.x += 1.f;

    const sf::Vector2f move = normalizeOrZero(input) * ghostSpeed * dt;

    const sf::Vector2f prev = ghostPos;
    ghostPos += move;

    ghostPos.x = clampf(ghostPos.x, 32.f, 1280.f - 32.f);
    ghostPos.y = clampf(ghostPos.y, 32.f, 720.f - 32.f);

    ghostHitbox.setPosition(ghostPos);

    for (const auto& b : blockers) {
        if (ghostHitbox.getGlobalBounds().intersects(b.getGlobalBounds())) {
            ghostPos = prev;
            ghostHitbox.setPosition(ghostPos);
            break;
        }
    }

    ghostSprite.setPosition(ghostPos);

    // swap grave1 to flowerbed after level 1 complete
    if (hasGraveTexture) {
        if (hasFlowerTexture && Levels::level1Complete) {
            grave1Sprite.setTexture(Assets::getTexture("flowerbed"), true);
        } else {
            grave1Sprite.setTexture(Assets::getTexture("grave"), true);
        }
    }

    // keep visuals aligned
    if (hasGraveTexture) {
        grave1Sprite.setPosition(grave1Rect.getPosition());
        grave2Sprite.setPosition(grave2Rect.getPosition());
        grave3Sprite.setPosition(grave3Rect.getPosition());
    }

    level1Label.setPosition(grave1Rect.getPosition().x, grave1Rect.getPosition().y + 70.f);
    level2Label.setPosition(grave2Rect.getPosition().x, grave2Rect.getPosition().y + 70.f);
    level3Label.setPosition(grave3Rect.getPosition().x, grave3Rect.getPosition().y + 70.f);
}

void CemeteryScene::render(sf::RenderWindow& window) {
    // grass base
    for (int y = 0; y < 12; ++y)
        for (int x = 0; x < 20; ++x) {
            grassTile.setPosition(x * 64.f, y * 64.f);
            window.draw(grassTile);
        }

    // stone cross lanes
    for (int y = 1; y < 11; ++y) {
        stoneTile.setPosition(9 * 64.f, y * 64.f);
        window.draw(stoneTile);
        stoneTile.setPosition(10 * 64.f, y * 64.f);
        window.draw(stoneTile);
    }
    for (int x = 6; x < 14; ++x) {
        stoneTile.setPosition(x * 64.f, 8 * 64.f);
        window.draw(stoneTile);
        stoneTile.setPosition(x * 64.f, 6 * 64.f);
        window.draw(stoneTile);
        stoneTile.setPosition(x * 64.f, 4 * 64.f);
        window.draw(stoneTile);
    }

    // sprites only (no rectangle draw)
    if (hasGraveTexture) {
        window.draw(grave1Sprite);
        window.draw(grave2Sprite);
        window.draw(grave3Sprite);
    }

    window.draw(level1Label);
    window.draw(level2Label);
    window.draw(level3Label);

    window.draw(ghostSprite);
    window.draw(banner);

    if (overlaps(ghostHitbox, grave1Rect) || overlaps(ghostHitbox, grave2Rect) || overlaps(ghostHitbox, grave3Rect)) {
        interact.setPosition(ghostPos.x - 30.f, ghostPos.y - 60.f);
        window.draw(interact);
    }
}

void CemeteryScene::handleEvent(sf::Event& event) {
    if (event.type != sf::Event::KeyPressed) return;

    if (event.key.code == sf::Keyboard::M) {
        Levels::muted = !Levels::muted;
        GameSystem::applyMute();
        return;
    }

    if (event.key.code == sf::Keyboard::E) {
        if (overlaps(ghostHitbox, grave1Rect)) {
            GameSystem::setActiveScene(Levels::level1);
        } else if (overlaps(ghostHitbox, grave2Rect)) {
            GameSystem::setActiveScene(Levels::level2);
        }
        return;
    }

    if (event.key.code == sf::Keyboard::Escape) {
        GameSystem::pauseMusic();
        Levels::pausedFrom = Levels::cemetery;
        GameSystem::setActiveScene(Levels::pause);
        return;
    }
}
