#include "assets/Assets.h"
#include <stdexcept>

std::unordered_map<std::string, sf::Texture> Assets::textures;

void Assets::loadTexture(const std::string& name, const std::string& path) {
    sf::Texture tex;
    if (!tex.loadFromFile(path))
        throw std::runtime_error("Failed to load texture: " + path);
    textures[name] = std::move(tex);
}

const sf::Texture& Assets::getTexture(const std::string& name) {
    return textures.at(name);
}
