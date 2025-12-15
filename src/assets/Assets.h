#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

class Assets {
public:
    // rquired assets
    static void loadTexture(const std::string& name, const std::string& path);

    // optional assets 
    static bool loadTextureSafe(const std::string& name, const std::string& path);

    static bool hasTexture(const std::string& name);
    static const sf::Texture& getTexture(const std::string& name);

private:
    static std::unordered_map<std::string, sf::Texture> textures;
};
