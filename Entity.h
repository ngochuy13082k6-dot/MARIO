#pragma once
#include <SFML/Graphics.hpp>
// Lớp cơ sở cho các thực thể di chuyển/vật lý (Player, Enemy)

class Entity
{
public:
    Entity() {}
    virtual ~Entity() {}

    // Hàm thuần ảo: Bắt buộc lớp con triển khai logic update
    virtual void update(float deltaTime) = 0;

    // Hàm ảo: Dùng để vẽ sprite
    virtual void render(sf::RenderTarget& target) {
        target.draw(mSprite);
    }

    sf::FloatRect getGlobalBounds() const { return mSprite.getGlobalBounds(); }
    sf::Vector2f getPosition() const { return mSprite.getPosition(); }

protected:
    sf::Sprite mSprite;
    sf::Texture mTexture;
};