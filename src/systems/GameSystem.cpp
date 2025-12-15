#include "systems/GameSystem.h"
#include "levels.h"

sf::RenderWindow GameSystem::window_;
sf::Clock GameSystem::clock_;
std::shared_ptr<Scene> GameSystem::current_scene_ = nullptr;

sf::Music GameSystem::music_;
std::string GameSystem::current_track_;

// Fixed virtual resolution
static const sf::Vector2f VIRTUAL_SIZE(1280.f, 720.f);

static sf::View makeLetterboxView(sf::Vector2u windowSize) {
    sf::View view({0.f, 0.f, VIRTUAL_SIZE.x, VIRTUAL_SIZE.y});

    const float windowRatio = windowSize.x / static_cast<float>(windowSize.y);
    const float viewRatio   = VIRTUAL_SIZE.x / VIRTUAL_SIZE.y;

    float sizeX = 1.f;
    float sizeY = 1.f;
    float posX  = 0.f;
    float posY  = 0.f;

    if (windowRatio > viewRatio) {
        sizeX = viewRatio / windowRatio;
        posX = (1.f - sizeX) / 2.f;
    } else {
        sizeY = windowRatio / viewRatio;
        posY = (1.f - sizeY) / 2.f;
    }

    view.setViewport({posX, posY, sizeX, sizeY});
    return view;
}

void GameSystem::setActiveScene(const std::shared_ptr<Scene>& scene) {
    current_scene_ = scene;
}

void GameSystem::quit() {
    window_.close();
}

sf::RenderWindow& GameSystem::window() {
    return window_;
}

// ---------------- MUSIC (single owner) ----------------

void GameSystem::applyMute() {
    // simplest + lab-safe: just volume 0/100
    music_.setVolume(Levels::muted ? 0.f : 100.f);
}

void GameSystem::playMusic(const std::string& path, bool loop) {
    // If same track already playing, do nothing
    if (current_track_ == path && music_.getStatus() == sf::Music::Playing) {
        applyMute();
        return;
    }

    // Stop previous, load new
    music_.stop();
    current_track_.clear();

    if (!music_.openFromFile(path)) {
        return; // fail silently (lab-style)
    }

    current_track_ = path;
    music_.setLoop(loop);
    applyMute();
    music_.play();
}

void GameSystem::stopMusic() {
    music_.stop();
    current_track_.clear();
}

void GameSystem::pauseMusic() {
    if (music_.getStatus() == sf::Music::Playing) {
        music_.pause();
    }
}

void GameSystem::resumeMusic() {
    if (music_.getStatus() != sf::Music::Playing && !current_track_.empty()) {
        applyMute();
        music_.play();
    }
}

// ---------------- LOOP ----------------

void GameSystem::start() {
    window_.create(sf::VideoMode(1280, 720), "Ghost Redemption", sf::Style::Default);
    window_.setFramerateLimit(60);

    window_.setView(makeLetterboxView(window_.getSize()));

    while (window_.isOpen()) {
        float dt = clock_.restart().asSeconds();
        processEvents();
        update(dt);
        render();
    }
}

void GameSystem::processEvents() {
    sf::Event event{};
    while (window_.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window_.close();

        if (event.type == sf::Event::Resized) {
            window_.setView(makeLetterboxView({event.size.width, event.size.height}));
        }

        if (current_scene_)
            current_scene_->handleEvent(event);
    }
}

void GameSystem::update(float dt) {
    if (current_scene_)
        current_scene_->update(dt);
}

void GameSystem::render() {
    window_.clear();
    if (current_scene_)
        current_scene_->render(window_);
    window_.display();
}
