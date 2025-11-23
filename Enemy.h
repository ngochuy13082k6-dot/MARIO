#pragma once
#include "Entity.h"
#include "Defines.h"
#include <vector>

class Enemy : public Entity // Kế thừa Entity
{
public:
    // Constructor nhận texture dùng chung
    Enemy(float x, float y, const sf::Texture& texture);
    void update(float deltaTime) override; // Logic di chuyển, trọng lực
    void checkCollisionWithPlatforms(const std::vector<sf::RectangleShape>& platforms);

    void die();
    bool isDead() const;  // Kiểm tra đã chết hẳn chưa
    bool isDying() const; // Đang trong trạng thái bị bẹp

private:
    float mSpeed;
    float mVelocityY;
    bool mMovingRight;
    bool mIsDead;
    float mDeathTimer;
};