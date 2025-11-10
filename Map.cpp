// *** DÒNG NÀY CỰC KỲ QUAN TRỌNG! ***
#include "Map.h"

Map::Map()
{
    // Hàm khởi tạo, để trống
}

// Tải hình ảnh mây
void Map::loadAssets()
{
    // TẢI 6 TEXTURE MÂY RIÊNG BIỆT (Lấy từ main() cũ)
    mCloudTextures.resize(6);
    for (int i = 0; i < 6; ++i) {
        if (!mCloudTextures[i].loadFromFile("Assets/cloud" + std::to_string(i + 1) + ".png")) {
            std::cerr << "Loi tai Assets/cloud" << std::to_string(i + 1) << ".png" << std::endl;
        }
    }
}

// Tạo lại platform và mây
void Map::reset()
{
    // --- PLATFORMS --- (Lấy từ main() cũ)
    // *** CHÚ Ý: ĐÂY LÀ NƠI TOÀN BỘ LEVEL DESIGN (CODE PLATFORM) CỦA BẠN ĐƯỢC CHUYỂN VÀO ***
    mPlatforms.clear(); // Xóa platform cũ (nếu có)
    sf::Color blockColor = sf::Color(101, 67, 33);

    // Platform mặt đất (luôn là index 0)
    sf::RectangleShape groundHitbox(sf::Vector2f(GAME_WIDTH, 50.0f));
    groundHitbox.setFillColor(blockColor);
    groundHitbox.setPosition(0.0f, WINDOW_HEIGHT - 50.f);
    mPlatforms.push_back(groundHitbox);

    // (Toàn bộ code data.platforms.push_back(...) của bạn nằm ở đây)
    mPlatforms.push_back(sf::RectangleShape(sf::Vector2f(150.f, 30.f))); mPlatforms.back().setFillColor(blockColor); mPlatforms.back().setPosition(250.f, WINDOW_HEIGHT - 150.f);
    mPlatforms.push_back(sf::RectangleShape(sf::Vector2f(200.f, 30.f))); mPlatforms.back().setFillColor(blockColor); mPlatforms.back().setPosition(550.f, WINDOW_HEIGHT - 250.f);
    mPlatforms.push_back(sf::RectangleShape(sf::Vector2f(100.f, 30.f))); mPlatforms.back().setFillColor(blockColor); mPlatforms.back().setPosition(350.f, WINDOW_HEIGHT - 350.f);
    mPlatforms.push_back(sf::RectangleShape(sf::Vector2f(100.f, 30.f))); mPlatforms.back().setFillColor(blockColor); mPlatforms.back().setPosition(600.f, WINDOW_HEIGHT - 450.f);
    mPlatforms.push_back(sf::RectangleShape(sf::Vector2f(100.f, 30.f))); mPlatforms.back().setFillColor(blockColor); mPlatforms.back().setPosition(900.f, WINDOW_HEIGHT - 150.f);
    mPlatforms.push_back(sf::RectangleShape(sf::Vector2f(100.f, 30.f))); mPlatforms.back().setFillColor(blockColor); mPlatforms.back().setPosition(1100.f, WINDOW_HEIGHT - 250.f);
    mPlatforms.push_back(sf::RectangleShape(sf::Vector2f(100.f, 30.f))); mPlatforms.back().setFillColor(blockColor); mPlatforms.back().setPosition(1300.f, WINDOW_HEIGHT - 350.f);
    mPlatforms.push_back(sf::RectangleShape(sf::Vector2f(300.f, 30.f))); mPlatforms.back().setFillColor(blockColor); mPlatforms.back().setPosition(1500.f, WINDOW_HEIGHT - 200.f);
    mPlatforms.push_back(sf::RectangleShape(sf::Vector2f(100.f, 30.f))); mPlatforms.back().setFillColor(blockColor); mPlatforms.back().setPosition(1750.f, WINDOW_HEIGHT - 300.f);
    mPlatforms.push_back(sf::RectangleShape(sf::Vector2f(100.f, 30.f))); mPlatforms.back().setFillColor(blockColor); mPlatforms.back().setPosition(1950.f, WINDOW_HEIGHT - 400.f);
    mPlatforms.push_back(sf::RectangleShape(sf::Vector2f(100.f, 30.f))); mPlatforms.back().setFillColor(blockColor); mPlatforms.back().setPosition(2150.f, WINDOW_HEIGHT - 500.f);
    mPlatforms.push_back(sf::RectangleShape(sf::Vector2f(120.f, 30.f))); mPlatforms.back().setFillColor(blockColor); mPlatforms.back().setPosition(400.f, WINDOW_HEIGHT - 550.f);
    mPlatforms.push_back(sf::RectangleShape(sf::Vector2f(100.f, 30.f))); mPlatforms.back().setFillColor(blockColor); mPlatforms.back().setPosition(750.f, WINDOW_HEIGHT - 350.f);
    mPlatforms.push_back(sf::RectangleShape(sf::Vector2f(100.f, 30.f))); mPlatforms.back().setFillColor(blockColor); mPlatforms.back().setPosition(1000.f, WINDOW_HEIGHT - 500.f);
    mPlatforms.push_back(sf::RectangleShape(sf::Vector2f(150.f, 30.f))); mPlatforms.back().setFillColor(blockColor); mPlatforms.back().setPosition(1200.f, WINDOW_HEIGHT - 150.f);
    mPlatforms.push_back(sf::RectangleShape(sf::Vector2f(100.f, 30.f))); mPlatforms.back().setFillColor(blockColor); mPlatforms.back().setPosition(1450.f, WINDOW_HEIGHT - 450.f);
    mPlatforms.push_back(sf::RectangleShape(sf::Vector2f(100.f, 30.f))); mPlatforms.back().setFillColor(blockColor); mPlatforms.back().setPosition(1650.f, WINDOW_HEIGHT - 550.f);
    mPlatforms.push_back(sf::RectangleShape(sf::Vector2f(150.f, 30.f))); mPlatforms.back().setFillColor(blockColor); mPlatforms.back().setPosition(1900.f, WINDOW_HEIGHT - 150.f);
    mPlatforms.push_back(sf::RectangleShape(sf::Vector2f(100.f, 30.f))); mPlatforms.back().setFillColor(blockColor); mPlatforms.back().setPosition(2050.f, WINDOW_HEIGHT - 300.f);
    mPlatforms.push_back(sf::RectangleShape(sf::Vector2f(100.f, 30.f))); mPlatforms.back().setFillColor(blockColor); mPlatforms.back().setPosition(2250.f, WINDOW_HEIGHT - 200.f);
    mPlatforms.push_back(sf::RectangleShape(sf::Vector2f(100.f, 30.f))); mPlatforms.back().setFillColor(blockColor); mPlatforms.back().setPosition(2400.f, WINDOW_HEIGHT - 400.f);

    // TẠO 6 SPRITE MÂY VÀ ĐẶT VỊ TRÍ CỐ ĐỊNH (Lấy từ main() cũ)
    mClouds.clear(); // Xóa mây cũ
    if (mCloudTextures[0].getSize().x > 0) { // Chỉ tạo nếu tải được hình
        sf::Sprite cloud1(mCloudTextures[0]); cloud1.setPosition(300.f, 100.f); cloud1.setScale(0.8f, 0.8f); mClouds.push_back(cloud1);
        sf::Sprite cloud2(mCloudTextures[1]); cloud2.setPosition(800.f, 150.f); cloud2.setScale(1.0f, 1.0f); mClouds.push_back(cloud2);
        sf::Sprite cloud3(mCloudTextures[2]); cloud3.setPosition(1300.f, 120.f); cloud3.setScale(0.9f, 0.9f); mClouds.push_back(cloud3);
        sf::Sprite cloud4(mCloudTextures[3]); cloud4.setPosition(1800.f, 100.f); cloud4.setScale(1.1f, 1.1f); mClouds.push_back(cloud4);
        sf::Sprite cloud5(mCloudTextures[4]); cloud5.setPosition(2200.f, 140.f); cloud5.setScale(0.7f, 0.7f); mClouds.push_back(cloud5);
        sf::Sprite cloud6(mCloudTextures[5]); cloud6.setPosition(2700.f, 110.f); cloud6.setScale(1.0f, 1.0f); mClouds.push_back(cloud6);
    }
}

// Vẽ platform (sẽ được gọi bởi Game::render, dùng camera chính)
void Map::render(sf::RenderTarget& target)
{
    // --- LOGIC VẼ TỪ renderGame() CŨ ---
    for (const auto& platform : mPlatforms) { target.draw(platform); }
}

// Vẽ mây (sẽ được gọi bởi Game::render, dùng camera mây)
void Map::renderParallax(sf::RenderTarget& target, sf::View& cloudView, const sf::View& camera)
{
    // --- LOGIC VẼ TỪ renderGame() CŨ ---
    // 1. Vẽ Lớp Mây (Parallax)
    // (Tính toán vị trí camera của mây để di chuyển chậm hơn camera chính, tạo ảo giác về chiều sâu)
    float cameraXCenterMin = WINDOW_WIDTH / 2.f; float cameraYCenterMin = WINDOW_HEIGHT / 2.f;
    float cameraYPosMin = GAME_HEIGHT - cameraYCenterMin;
    float cameraXPosMin = cameraXCenterMin;
    // Mây di chuyển 50% tốc độ camera X, 80% tốc độ camera Y
    float cloudCamX = cameraXPosMin + (camera.getCenter().x - cameraXPosMin) * 0.5f;
    float cloudCamY = cameraYPosMin + (camera.getCenter().y - cameraYPosMin) * 0.8f;
    cloudView.setCenter(cloudCamX, cloudCamY); // Cập nhật camera mây
    target.setView(cloudView); // Áp dụng camera mây
    for (const auto& cloud : mClouds) {
        target.draw(cloud); // Vẽ mây
    }
}

// Hàm Get cho Player
const std::vector<sf::RectangleShape>& Map::getPlatforms() const
{
    return mPlatforms;
}