#include "scenes/Level1Scene.h"
#include "assets/Assets.h"
#include "levels.h"
#include "systems/GameSystem.h"
#include <cmath>
#include <cstdlib>

namespace {
    sf::Vector2f normalize(sf::Vector2f v) {
        const float len2 = v.x * v.x + v.y * v.y;
        if (len2 <= 0.0001f) return {0.f, 0.f};
        const float inv = 1.0f / std::sqrt(len2);
        return {v.x * inv, v.y * inv};
    }

    float dot(sf::Vector2f a, sf::Vector2f b) { return a.x * b.x + a.y * b.y; }

    sf::FloatRect inset(sf::FloatRect r, float amount) {
        r.left += amount;
        r.top += amount;
        r.width -= amount * 2.f;
        r.height -= amount * 2.f;
        return r;
    }

    float dist2(sf::Vector2f p, sf::Vector2f q) {
        const sf::Vector2f d = p - q;
        return d.x * d.x + d.y * d.y;
    }
}

void Level1Scene::playClick() {
    if (hasClick && !Levels::muted) {
        clickSound.play();
    }
}

Level1Scene::Level1Scene() {
    // Textures
    floorTile.setTexture(Assets::getTexture("stone"));
    floorTile.setTextureRect({0, 0, 32, 32});
    floorTile.setScale(2.f, 2.f);

    ghostSprite.setTexture(Assets::getTexture("ghost"));
    ghostSprite.setTextureRect({0, 0, 32, 32});
    ghostSprite.setOrigin(16.f, 16.f);
    ghostSprite.setScale(2.f, 2.f);

    hasDogTexture = Assets::hasTexture("dog");

    // Font/UI
    (void)font.loadFromFile("resources/fonts/Kenney Future.ttf");
    hud.setFont(font);
    hud.setCharacterSize(20);
    hud.setFillColor(sf::Color(230, 230, 230));
    hud.setString("REACH DOOR | AVOID POOLS + DOGS   [R] Restart  [M] Mute  [ESC] Pause");
    hud.setPosition(20.f, 20.f);

    // Audio
    hasClick = clickBuffer.loadFromFile("resources/sfx/ui/click.ogg");
    if (hasClick) {
        clickSound.setBuffer(clickBuffer);
        clickSound.setVolume(60.f);
    }

    // Player
    player.setSize({24.f, 24.f});
    player.setFillColor(sf::Color(255, 255, 255, 0));
    player.setOrigin(12.f, 12.f);
    playerStart = {80.f, 620.f};
    player.setPosition(playerStart);
    ghostSprite.setPosition(playerStart);

    // Pool border texture
    wallTex = &Assets::getTexture("wall");

    // Door goal
    hasDoorClosed = Assets::hasTexture("closed_door");
    hasDoorOpen = Assets::hasTexture("open_door");

    if (hasDoorClosed) {
        doorSprite.setTexture(Assets::getTexture("closed_door"));
        const auto b = doorSprite.getLocalBounds();
        doorSprite.setOrigin(b.width * 0.5f, b.height * 0.5f);
        doorSprite.setScale(2.f, 2.f);
        doorSprite.setPosition({1180.f, 90.f});
    }

    // Pools
    addPool({200.f, 160.f}, {180.f, 120.f}, "POOL 1");
    addPool({520.f, 360.f}, {240.f, 140.f}, "POOL 2");

    // Pool 3 moved below the door
    addPool({1040.f, 170.f}, {200.f, 120.f}, "POOL 3");

    // Dogs: go almost full screen (leave 1 tile margin each side)
    addDog({64.f, 300.f}, {1280.f - 64.f, 300.f}, 160.f);            // horizontal full length
    addDog({980.f, 64.f}, {980.f, 720.f - 64.f}, 140.f);             // vertical full height
}

void Level1Scene::addPool(const sf::Vector2f& pos, const sf::Vector2f& size, const std::string& name) {
    Pool p;
    p.water.setPosition(pos);
    p.water.setSize(size);
    p.water.setFillColor(sf::Color(18, 70, 190));

    p.label.setFont(font);
    p.label.setString(name);
    p.label.setCharacterSize(28);
    p.label.setFillColor(sf::Color(245, 245, 245));
    p.label.setOutlineThickness(2.f);
    p.label.setOutlineColor(sf::Color(0, 0, 0, 170));

    const sf::FloatRect tb = p.label.getLocalBounds();
    p.label.setOrigin(tb.left + tb.width * 0.5f, tb.top + tb.height * 0.5f);
    p.label.setPosition(pos.x + size.x * 0.5f, pos.y + size.y * 0.5f);

    pools.push_back(p);
}

void Level1Scene::addDog(const sf::Vector2f& a, const sf::Vector2f& b, float speed) {
    Dog d;
    d.body.setSize({28.f, 18.f});
    d.body.setFillColor(sf::Color(255, 255, 255, 0));
    d.body.setOrigin(14.f, 9.f);
    d.a = a;
    d.b = b;

    const float jitter = static_cast<float>((std::rand() % 31) - 15);
    d.speed = speed + jitter;

    d.body.setPosition(a);
    d.goingToB = true;
    d.waitTimer = 0.f;
    d.dir = normalize(b - a);
    dogs.push_back(d);

    if (hasDogTexture) {
        sf::Sprite s;
        s.setTexture(Assets::getTexture("dog"));
        s.setTextureRect({0, 0, 32, 32});
        s.setOrigin(16.f, 16.f);
        s.setScale(2.f, 2.f);
        s.setPosition(a);
        dogSprites.push_back(s);
    }
}

void Level1Scene::drawPoolBorder(sf::RenderWindow& window, const sf::RectangleShape& water) const {
    if (!wallTex) return;

    constexpr float TILE = 64.f;

    sf::Sprite t(*wallTex);
    t.setTextureRect({0, 0, 32, 32});
    t.setScale(2.f, 2.f);

    const sf::FloatRect b = water.getGlobalBounds();

    const float left = b.left - TILE;
    const float right = b.left + b.width;
    const float top = b.top - TILE;
    const float bottom = b.top + b.height;

    for (float x = left; x <= right; x += TILE) {
        t.setPosition(x, top);
        window.draw(t);
        t.setPosition(x, bottom);
        window.draw(t);
    }
    for (float y = b.top; y < bottom; y += TILE) {
        t.setPosition(left, y);
        window.draw(t);
        t.setPosition(right, y);
        window.draw(t);
    }
}

void Level1Scene::reset() { resetLevel(); }

void Level1Scene::resetLevel() {
    doorOpened = false;
    doorOpenTimer = 0.f;

    if (hasDoorClosed) {
        doorSprite.setTexture(Assets::getTexture("closed_door"), true);
    }

    player.setPosition(playerStart);
    ghostSprite.setPosition(playerStart);

    for (size_t i = 0; i < dogs.size(); ++i) {
        dogs[i].body.setPosition(dogs[i].a);
        dogs[i].goingToB = true;
        dogs[i].waitTimer = 0.f;
        dogs[i].dir = normalize(dogs[i].b - dogs[i].a);
        if (hasDogTexture && i < dogSprites.size()) {
            dogSprites[i].setPosition(dogs[i].a);
        }
    }
}

void Level1Scene::win() {
    Levels::level1Complete = true;
    playClick();
    GameSystem::setActiveScene(Levels::cemetery);
}

void Level1Scene::handleEvent(sf::Event& event) {
    if (event.type != sf::Event::KeyPressed) return;

    if (event.key.code == sf::Keyboard::M) {
        Levels::muted = !Levels::muted;
        return;
    }
    if (event.key.code == sf::Keyboard::R) {
        playClick();
        resetLevel();
        return;
    }
    if (event.key.code == sf::Keyboard::Escape) {
        Levels::pausedFrom = Levels::level1;
        GameSystem::setActiveScene(Levels::pause);
    }
}

void Level1Scene::update(float dt) {
    // if door is opening, wait a moment then leave
    if (doorOpened) {
        doorOpenTimer -= dt;
        if (doorOpenTimer <= 0.f) {
            win();
            return;
        }
    }

    sf::Vector2f input(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) input.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) input.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) input.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) input.x += 1.f;

    player.move(normalize(input) * playerSpeed * dt);
    ghostSprite.setPosition(player.getPosition());

    const sf::FloatRect playerHit = inset(player.getGlobalBounds(), 4.f);

    // Dogs: patrol + prevent entering pools
    for (size_t i = 0; i < dogs.size(); ++i) {
        auto& d = dogs[i];
        const sf::Vector2f prev = d.body.getPosition();

        if (d.waitTimer > 0.f) {
            d.waitTimer -= dt;
        } else {
            d.body.move(d.dir * d.speed * dt);
        }

        const sf::Vector2f pos = d.body.getPosition();
        const sf::Vector2f target = d.goingToB ? d.b : d.a;
        const sf::Vector2f start = d.goingToB ? d.a : d.b;

        const sf::Vector2f seg = target - start;
        const sf::Vector2f toTarget = target - pos;
        const float passed = dot(toTarget, seg);

        if (dist2(pos, target) < (8.f * 8.f) || passed < 0.f) {
            d.body.setPosition(target);
            d.goingToB = !d.goingToB;
            const sf::Vector2f nextTarget = d.goingToB ? d.b : d.a;
            d.dir = normalize(nextTarget - target);
            d.waitTimer = 0.35f;
        }

        bool hitPool = false;
        for (const auto& p : pools) {
            const sf::FloatRect poolHit = inset(p.water.getGlobalBounds(), 2.f);
            if (d.body.getGlobalBounds().intersects(poolHit)) {
                hitPool = true;
                break;
            }
        }
        if (hitPool) {
            d.body.setPosition(prev);
            d.dir = {-d.dir.x, -d.dir.y};
            d.goingToB = !d.goingToB;
            d.waitTimer = 0.25f;
        }

        if (hasDogTexture && i < dogSprites.size()) {
            dogSprites[i].setPosition(d.body.getPosition());
            const float sx = std::abs(dogSprites[i].getScale().x);
            dogSprites[i].setScale((d.dir.x < 0.f) ? -sx : sx, dogSprites[i].getScale().y);
        }
    }

    // Player vs pools
    for (const auto& p : pools) {
        const sf::FloatRect poolHit = inset(p.water.getGlobalBounds(), 2.f);
        if (playerHit.intersects(poolHit)) {
            playClick();
            resetLevel();
            return;
        }
    }

    // Player vs dogs
    for (const auto& d : dogs) {
        if (playerHit.intersects(d.body.getGlobalBounds())) {
            playClick();
            resetLevel();
            return;
        }
    }

    // Player vs door (open it, then exit after a short delay)
    if (!doorOpened && hasDoorClosed) {
        const sf::FloatRect doorHit = doorSprite.getGlobalBounds();
        if (playerHit.intersects(doorHit)) {
            doorOpened = true;
            doorOpenTimer = 0.6f;
            playClick();

            if (hasDoorOpen) {
                doorSprite.setTexture(Assets::getTexture("open_door"), true);
            }
        }
    }
}

void Level1Scene::render(sf::RenderWindow& window) {
    for (int y = 0; y < 12; ++y) {
        for (int x = 0; x < 20; ++x) {
            floorTile.setPosition(x * 64.f, y * 64.f);
            window.draw(floorTile);
        }
    }

    // pools with borders + labels
    for (const auto& p : pools) {
        drawPoolBorder(window, p.water);
        window.draw(p.water);
        window.draw(p.label);
    }

    // door
    if (hasDoorClosed) {
        window.draw(doorSprite);
    }

    // dogs
    if (hasDogTexture) {
        for (const auto& s : dogSprites) window.draw(s);
    } else {
        for (const auto& d : dogs) window.draw(d.body);
    }

    window.draw(ghostSprite);
    window.draw(hud);
}
