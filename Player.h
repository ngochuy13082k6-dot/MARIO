#pragma once
#include "Defines.h"

class Player
{
public:
    Player();
    void loadAssets();
    void reset(bool resetAllStats);
    void handleEvent(sf::Event event);
    void update(float deltaTime, const std::vector<sf::RectangleShape>& platforms);
    void render(sf::RenderTarget& target);

    sf::Vector2f getVelocity() const { return mVelocity; }

    // --- CÁC HÀM CỘNG/TRỪ ĐIỂM VÀ MẠNG ---
    void takeDamage();
    void addScore(int points);
    void addCoin(int amount);       // <-- HÀM BỊ THIẾU
    void addLife();                 // <-- HÀM BỊ THIẾU
    void collectPowerup();          // <-- HÀM BỊ THIẾU

    // --- CÁC SETTER BỊ THIẾU (dùng cho logic Item/Reset) ---
    void setScore(int score);
    void setCoins(int coins);
    void setLives(int lives);
    void setOnGround(bool onGround);
    void setGameOver();             // <-- HÀM BỊ THIẾU

    // Getters
    sf::FloatRect getGlobalBounds() const;
    sf::Vector2f getPosition() const;
    sf::Vector2f getCenter() const;
    bool isHit() const;
    int getScore() const;
    int getCoins() const;
    int getLives() const;
    bool isDead() const;

private:
    void updateEffects(float deltaTime);
    void updateAnimation(float deltaTime, bool isMoving);
    void applyPhysics(float deltaTime);
    void checkCollisions(const std::vector<sf::RectangleShape>& platforms, sf::Vector2f displacement);

    sf::RectangleShape mHitbox;
    sf::Sprite mSprite;
    sf::Texture mTexture;
    sf::SoundBuffer mJumpBuffer;
    sf::Sound mJumpSound;

    sf::Vector2f mVelocity = { 0.f, 0.f };
    // --- CÁC BIẾN THÀNH VIÊN BỊ THIẾU GÂY LỖI "UNDEFINED IDENTIFIER" ---
    int mScore = 0;
    int mCoinsCollected = 0;
    int mLives = 5;
    bool mOnGround = false;
    bool mFacingRight = true;
    bool mIsDead = false;

    bool mPlayerHit = false;
    float mPlayerHitTimer = 0.f; // mPlayerHitTimer
    bool mPlayerPowerup = false; // mPlayerPowerup
    float mPlayerPowerupTimer = 0.f; // mPlayerPowerupTimer

    int mCurrentAnimationFrame = IDLE_FRAME_INDEX;
    float mAnimTimer = 0.f;
};