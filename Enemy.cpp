#include "Enemy.h"
#include "Defines.h" // Cần thiết để truy cập GRAVITY và GAME_HEIGHT
#include <iostream>

// Constructor: Nhận texture dùng chung (từ Game.cpp) và đặt vị trí ban đầu
Enemy::Enemy(float x, float y, const sf::Texture& texture)
{
    mSprite.setTexture(texture); // Dùng texture chung
    mSprite.setPosition(x, y);
    mSprite.setScale(0.25f, 0.25f); // Kích thước Goomba

    // Đặt gốc tọa độ ở CHÂN (bottom center)
    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(bounds.width / 2.f, bounds.height);

    mSpeed = 2.0f;
    mVelocityY = 0.0f;
    mMovingRight = false; // Mặc định Goomba đi sang trái
    mIsDead = false;
    mDeathTimer = 0.f;
}

// Hàm Update: Xử lý vật lý và thời gian chết
void Enemy::update(float deltaTime)
{
    if (mIsDead) {
        mDeathTimer += deltaTime;
        return; // Đã chết thì chỉ đếm timer
    }

    mVelocityY += GRAVITY; // Áp dụng trọng lực
    float moveX = (mMovingRight ? mSpeed : -mSpeed); // Tính toán hướng di chuyển
    mSprite.move(moveX * deltaTime * 60.f, mVelocityY * deltaTime * 60.f);

    // Logic rơi khỏi map
    if (mSprite.getPosition().y > GAME_HEIGHT + 100) {
        mDeathTimer = 10.0f; // Chết hẳn ngay lập tức (để bị xóa)
        mIsDead = true;
    }
}

// Hàm kiểm tra va chạm với Platform (Dùng để quay đầu và đứng trên đất)
void Enemy::checkCollisionWithPlatforms(const std::vector<sf::RectangleShape>& platforms)
{
    if (mIsDead) return;

    sf::FloatRect myBounds = mSprite.getGlobalBounds();

    for (const auto& plat : platforms)
    {
        sf::FloatRect platBounds = plat.getGlobalBounds();
        if (myBounds.intersects(platBounds))
        {
            // Va chạm bên dưới (đứng trên đất)
            if (myBounds.top + myBounds.height < platBounds.top + 15.f && mVelocityY >= 0)
            {
                mVelocityY = 0.f;
                mSprite.setPosition(mSprite.getPosition().x, platBounds.top);

                // AI: Tự động quay đầu nếu chạm mép platform
                if (mMovingRight && mSprite.getPosition().x > platBounds.left + platBounds.width) { mMovingRight = false; }
                else if (!mMovingRight && mSprite.getPosition().x < platBounds.left) { mMovingRight = true; }
            }
            // Va chạm ngang (đụng tường/platform) -> Quay đầu
            else if (myBounds.top > platBounds.top)
            {
                if (mMovingRight && myBounds.left + myBounds.width > platBounds.left) { mMovingRight = false; } // Đụng phải
                else if (!mMovingRight && myBounds.left < platBounds.left + platBounds.width) { mMovingRight = true; } // Đụng trái
            }
        }
    }
}

// Kích hoạt trạng thái chết (bị bẹp)
void Enemy::die() {
    if (!mIsDead) {
        mIsDead = true;
        mSprite.setScale(mSprite.getScale().x, mSprite.getScale().y * 0.5f); // Hiệu ứng bị bẹp
    }
}

// Kiểm tra: Goomba đã chết (đã bị bẹp) chưa
bool Enemy::isDying() const { return mIsDead; }

// Kiểm tra: Goomba đã hết thời gian bẹp và có thể xóa khỏi vector (Game.cpp) chưa
bool Enemy::isDead() const { return mIsDead && mDeathTimer > 0.5f; }