#pragma once
#include "Defines.h"

class Player;

class Item
{
public:
    Item();
    void loadAssets();
    // Reset nhận thêm vị trí cờ (flagBounds) để tránh va chạm Item
    void reset(int level, const std::vector<sf::RectangleShape>& platforms, sf::FloatRect flagBounds);
    void update(float deltaTime, Player& player);
    void render(sf::RenderTarget& target);

private:
    void updateActiveRewards(float deltaTime, Player& player);
    void checkCollisions(Player& player); // Va chạm Item-Player

    void spawnCoins(int count, const std::vector<sf::RectangleShape>& platforms, sf::FloatRect flagBounds);
    void spawnBoxes(int count, const std::vector<sf::RectangleShape>& platforms, sf::FloatRect flagBounds);
    void spawnFireFlowers(int count, const std::vector<sf::RectangleShape>& platforms, const sf::RectangleShape& ground, sf::FloatRect flagBounds);
    sf::Vector2f normalize(const sf::Vector2f& source); // Chuẩn hóa vector

    // --- Biến THÀNH VIÊN (FIX: Đảm bảo các biến này có mặt) ---
    sf::Texture mCoinTexture;
    sf::Texture mMysteryBoxTexture;
    sf::Texture mMushroomTexture;
    sf::Texture mFireFlowerTexture;
    sf::SoundBuffer mCoinBuffer;
    sf::Sound mCoinSound;
    sf::SoundBuffer mPowerupBuffer;
    sf::Sound mPowerupSound;
    sf::SoundBuffer mDamageBuffer;
    sf::Sound mDamageSound;

    std::vector<sf::Sprite> mCoins; // mCoins
    std::vector<MysteryBox> mMysteryBoxes; // mMysteryBoxes
    std::vector<Reward> mActiveRewards; // mActiveRewards
    std::vector<sf::Sprite> mGroundFireFlowers; // mGroundFireFlowers
};