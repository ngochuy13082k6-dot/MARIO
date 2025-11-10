#pragma once
#include "Defines.h"

// Class Map quản lý nền (platform) và mây (background)
class Map
{
public:
    Map();
    void loadAssets(); // Tải hình ảnh mây
    void reset(); // Tạo lại platform và mây
    void render(sf::RenderTarget& target); // Vẽ platform (dùng camera chính)

    // Vẽ mây (dùng camera mây riêng để tạo hiệu ứng parallax)
    // Cần 'camera' (camera chính) để biết vị trí Player
    // Cần 'cloudView' (camera mây) để cập nhật vị trí mới
    void renderParallax(sf::RenderTarget& target, sf::View& cloudView, const sf::View& camera);

    // Hàm Get để Player biết platform ở đâu để va chạm
    // Trả về tham chiếu hằng (`const&`) để tăng hiệu suất (không cần copy vector)
    const std::vector<sf::RectangleShape>& getPlatforms() const;

private:
    std::vector<sf::RectangleShape> mPlatforms; // Danh sách các platform
    std::vector<sf::Texture> mCloudTextures; // Danh sách hình ảnh mây
    std::vector<sf::Sprite> mClouds; // Các sprite mây trong màn chơi
};