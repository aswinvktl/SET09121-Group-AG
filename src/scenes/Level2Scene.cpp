#include "scenes/Level2Scene.h"
#include "levels.h"
#include "systems/GameSystem.h"
#include "assets/Assets.h"
#include <cmath>

//Making sure player won't move faster diagonally
static sf::Vector2f normalizeOrZero(sf::Vector2f v) {
    float len2 = v.x * v.x + v.y * v.y;
    if (len2 <= 0.0001f) return { 0.f, 0.f };
    float inv = 1.0f / std::sqrt(len2);
    return { v.x * inv, v.y * inv };
}


Level2Scene::Level2Scene() {

    // Cyborg
    playerCyborg.setTexture(Assets::getTexture("cyborg"));
    playerCyborg.setTextureRect(sf::IntRect(0, 0, 32, 32));
    playerCyborg.setOrigin(16.f, 16.f);
    playerCyborg.setScale(2.f, 2.f);
    playerPosition = { 120.f, 640.f };
    playerCyborg.setPosition(playerPosition);

    // Player hitbox
    playerHitbox.setSize({ 46.f, 46.f });
    playerHitbox.setOrigin(23.f, 23.f);
    playerHitbox.setPosition(playerPosition);
    playerHitbox.setFillColor(sf::Color(255, 0, 0, 0)); //Making it invisible




    // Walls

    auto addWall = [&](float x, float y, float w, float h) {
        sf::RectangleShape r;
        r.setPosition({ x, y });
        r.setSize({ w, h });
        r.setFillColor(sf::Color(55, 55, 75));
        r.setOutlineThickness(2.f);
        r.setOutlineColor(sf::Color(0, 0, 0, 90));
        walls.push_back(r);
        };

    // Border walls
    addWall(0.f, 0.f, 1280.f, 32.f); //top
    addWall(0.f, 688.f, 1280.f, 32.f); // bottom
    addWall(0.f, 0.f, 32.f, 720.f); //left
    addWall(1248.f, 0.f, 32.f, 720.f); // right


    //Simple maze like coridors
    
    // Thickness
    const float T = 24.f;


    addWall(200.f, 550.f, 950.f, T); 
    addWall(1150.f, 120.f, T, 455.f);


    addWall(200.f, 180.f, T, 380.f);

    addWall(400.f, 0.f, T, 400.f);

    addWall(400.f, 400.f, 670.f, T);

    addWall(500.f, 200.f, 670.f, T);


    // Charger Stations


    auto makeCharger = [&](float x, float y) {
        sf::Sprite s;
        s.setTexture(Assets::getTexture("charger"));
        s.setTextureRect(sf::IntRect(0, 0, 32, 32));
        s.setOrigin(16.f, 16.f);
        s.setScale(2.f, 2.f);
        s.setPosition(x, y);
        chargers.push_back(s);
        };

    chargers.clear();


    makeCharger(270.f, 360.f);
    makeCharger(180.f, 680.f -60.f);
    makeCharger(500.f, 110.f);

    makeCharger(1100.f, 500.f);



    // Battery UI
    battery.setPosition({ 20.f, 20.f });
    battery.setSize({ 300.f, 18.f });
    battery.setFillColor(sf::Color(30, 30, 30));

    batteryFill.setPosition({ 20.f, 20.f });
    batteryFill.setSize({ 300.f, 18.f });
    batteryFill.setFillColor(sf::Color(80, 200, 120));


    // Mechanic
    mechanic.setTexture(Assets::getTexture("mechanic"));
    mechanic.setTextureRect(sf::IntRect(0, 0, 32, 32));
    mechanic.setOrigin(16.f, 16.f);
    mechanic.setScale(2.f, 2.f);
    mechanic.setPosition({ 1210.f, 90.f });


    (void)font.loadFromFile("resources/fonts/Kenney Future.ttf");
    pressEText.setFont(font);
    pressEText.setCharacterSize(22);
    pressEText.setFillColor(sf::Color(255, 255, 255));
    pressEText.setOutlineThickness(2.f);
    pressEText.setOutlineColor(sf::Color(0, 0, 0, 160));
    pressEText.setString("Press E");


}

void Level2Scene::update(float dt) {
    // Movement
    sf::Vector2f input(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) input.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) input.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) input.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) input.x += 1.f;





    sf::Vector2f dir = normalizeOrZero(input);
    sf::Vector2f delta = dir * playerSpeed * dt;

    sf::Vector2f prevPos = playerPosition;

    // Move X
    playerPosition.x += delta.x;
    playerHitbox.setPosition(playerPosition);

    for (const auto& w : walls) {
        if (playerHitbox.getGlobalBounds().intersects(w.getGlobalBounds())) {
            playerPosition.x = prevPos.x;
            playerHitbox.setPosition(playerPosition);
            break;
        }
    }

    // Move Y
    playerPosition.y += delta.y;
    playerHitbox.setPosition(playerPosition);

    for (const auto& w : walls) {
        if (playerHitbox.getGlobalBounds().intersects(w.getGlobalBounds())) {
            playerPosition.y = prevPos.y;
            playerHitbox.setPosition(playerPosition);
            break;
        }
    }

    // Clamp to window bounds
    if (playerPosition.x < 32.f) playerPosition.x = 32.f;
    if (playerPosition.x > 1280.f - 32.f) playerPosition.x = 1280.f - 32.f;
    if (playerPosition.y < 32.f) playerPosition.y = 32.f;
    if (playerPosition.y > 720.f - 32.f) playerPosition.y = 720.f - 32.f;

    playerCyborg.setPosition(playerPosition);
    playerHitbox.setPosition(playerPosition);


    //Drain battry over time
    energy -= drainRate * dt;
    if (energy < 0.f) {
        energy = 0.f;
    }

    if (energy <= 0.f) {
        energy = 100.f;
        playerPosition = { 120.f, 640.f };
        playerCyborg.setPosition(playerPosition);
        playerHitbox.setPosition(playerPosition);
    }

    //Update UI fill width
    float pct = energy / 100.f;
    batteryFill.setSize({ 300.f * pct, 18.f });


    // Win condition
    if (playerHitbox.getGlobalBounds().intersects(mechanic.getGlobalBounds())) {
        Levels::level2Complete = true;
        GameSystem::setActiveScene(Levels::cemetery);
        return;
    }

}

void Level2Scene::render(sf::RenderWindow& window) {
    window.clear(sf::Color(15, 15, 25));

    for (auto& w : walls) window.draw(w);
    for (auto& c : chargers)window.draw(c);

    window.draw(mechanic);

    window.draw(playerCyborg);
    window.draw(battery);
    window.draw(batteryFill);

    bool nearCharger = false;
    sf::Vector2f nearPos;

    for (auto& c : chargers) {
        if (playerHitbox.getGlobalBounds().intersects(c.getGlobalBounds())) {
            nearCharger = true;
            nearPos = c.getPosition();
            break;
        }
    }

    if (nearCharger) {
        pressEText.setPosition(nearPos.x - 40.f, nearPos.y - 70.f);
        window.draw(pressEText);
    }

}

void Level2Scene::handleEvent(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Escape) {

        Levels::pausedFrom = Levels::level2;
        GameSystem::setActiveScene(Levels::pause);
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E) {

        // If player interacts witht thestation charge up to 100

        for (auto& c : chargers) {
            if (playerHitbox.getGlobalBounds().intersects(c.getGlobalBounds())) {
                energy = 100.f;
                break;
            }
        }

    }

}
