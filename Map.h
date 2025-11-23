#pragma once
#include "Defines.h"
#include <vector>

class Map
{
public:
    Map();
    void loadAssets();
    void reset(int level); // Tải layout map theo Level
    void render(sf::RenderTarget& target);
    void renderParallax(sf::RenderWindow& window, sf::View& cloudView, const sf::View& cameraView);

    const std::vector<sf::RectangleShape>& getPlatforms() const;
    sf::FloatRect getGoalBounds() const; // Vùng va chạm của cột cờ (Sprite)

private:
    std::vector<sf::RectangleShape> mPlatforms;
    std::vector<sf::Texture> mCloudTextures;
    std::vector<sf::Sprite> mClouds;

    // Biến cho Flag
    sf::Texture mFlagTexture;
    sf::Sprite mFlagSprite; // Sprite cột cờ
};