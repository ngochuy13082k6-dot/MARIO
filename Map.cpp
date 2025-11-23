#include "Map.h"
#include "Defines.h" 
#include <fstream>
#include <iostream>
#include <vector>

Map::Map() {}

// --- LOGIC LOAD ASSETS ---
void Map::loadAssets()
{
    // Load Textures Mây (Cloud Textures)
    mCloudTextures.resize(6);
    for (int i = 0; i < 6; ++i)
    {
        if (!mCloudTextures[i].loadFromFile("Assets/cloud" + std::to_string(i + 1) + ".png"))
            std::cerr << "Loi tai Assets/cloud" << i + 1 << ".png\n";
    }

    // Load Ảnh Cột Cờ (Goal Flag)
    if (!mFlagTexture.loadFromFile("Assets/flag.png"))
    {
        std::cerr << "Loi tai Assets/flag.png\n";
    }
}

// --- LOGIC RESET VÀ TẢI MAP ---
void Map::reset(int level)
{
    mPlatforms.clear(); // Xóa platforms cũ
    mClouds.clear();    // Xóa mây cũ

    // 1. Thêm mặt đất (Ground)
    sf::RectangleShape ground(sf::Vector2f(GAME_WIDTH, 50.f));
    ground.setFillColor(sf::Color(101, 67, 33)); // Màu nâu
    ground.setPosition(0.f, GAME_HEIGHT - 50.f); // Đặt ở đáy màn hình (Y = 670)
    mPlatforms.push_back(ground);

    // 2. Đọc platforms từ file (Logic chính tải map tĩnh)
    std::string mapFile = "Assets/level" + std::to_string(level) + "_map.txt";
    std::ifstream file(mapFile);
    if (!file.is_open())
    {
        std::cerr << "Khong the mo file map: " << mapFile << std::endl;
        return;
    }

    float x, y, w, h;
    while (file >> x >> y >> w >> h) // Đọc X Y Width Height
    {
        sf::RectangleShape platform(sf::Vector2f(w, h));
        platform.setPosition(x, y);
        platform.setFillColor(sf::Color(101, 67, 33));
        mPlatforms.push_back(platform);
    }
    file.close();

    // 3. Đặt vị trí Cột Cờ (Flag Sprite)
    mFlagSprite.setTexture(mFlagTexture);
    mFlagSprite.setScale(0.1f, 0.1f); // Scale theo yêu cầu

    // Tính toán vị trí Y: Đặt cờ đứng chính xác trên mặt đất
    float flagHeight = mFlagSprite.getGlobalBounds().height;
    // Y của cờ = Vị trí Y mặt đất (670) - Chiều cao cờ
    mFlagSprite.setPosition(GAME_WIDTH - 200.f, (GAME_HEIGHT - 50.f) - flagHeight);

    // 4. Tạo các Sprite Mây (Parallax - Vị trí cố định)
    if (!mCloudTextures.empty() && mCloudTextures[0].getSize().x > 0)
    {
        sf::Sprite cloud1(mCloudTextures[0]); cloud1.setPosition(300.f, 100.f); cloud1.setScale(0.8f, 0.8f); mClouds.push_back(cloud1);
        sf::Sprite cloud2(mCloudTextures[1]); cloud2.setPosition(800.f, 150.f); cloud2.setScale(1.f, 1.f); mClouds.push_back(cloud2);
        sf::Sprite cloud3(mCloudTextures[2]); cloud3.setPosition(1300.f, 120.f); cloud3.setScale(0.9f, 0.9f); mClouds.push_back(cloud3);
    }
}

// --- LOGIC VẼ (RENDER) ---
void Map::render(sf::RenderTarget& target)
{
    // Vẽ tất cả platform
    for (auto& plat : mPlatforms) target.draw(plat);
    // Vẽ cột cờ (Flag)
    target.draw(mFlagSprite);
}

// Hàm xử lý hiệu ứng Parallax cho Mây
void Map::renderParallax(sf::RenderWindow& window, sf::View& cloudView, const sf::View& cameraView)
{
    float cameraXCenterMin = WINDOW_WIDTH / 2.f;
    float cameraYCenterMin = WINDOW_HEIGHT / 2.f;
    float cameraYPosMin = GAME_HEIGHT - cameraYCenterMin;
    float cameraXPosMin = cameraXCenterMin;

    // Tính toán vị trí CloudView (di chuyển chậm hơn camera chính - 0.5x tốc độ)
    float cloudCamX = cameraXCenterMin + (cameraView.getCenter().x - cameraXPosMin) * 0.5f;
    float cloudCamY = cameraYCenterMin + (cameraView.getCenter().y - cameraYCenterMin) * 0.8f;

    // Giới hạn camera Y cho map 720px
    if (cloudCamY < cameraYCenterMin) cloudCamY = cameraYCenterMin;
    if (cloudCamY > cameraYPosMin) cloudCamY = cameraYPosMin;

    cloudView.setCenter(cloudCamX, cloudCamY);
    window.setView(cloudView); // Dùng View của mây để vẽ

    for (auto& cloud : mClouds) window.draw(cloud);
}

// Trả về Vector chứa tất cả Platform
const std::vector<sf::RectangleShape>& Map::getPlatforms() const { return mPlatforms; }

// Trả về vùng va chạm của cột cờ (dùng cho logic Win Game)
sf::FloatRect Map::getGoalBounds() const
{
    return mFlagSprite.getGlobalBounds();
}