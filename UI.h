#pragma once
#include "Defines.h"

// Class UI quản lý mọi thứ vẽ lên màn hình (không di chuyển theo camera)
// Bao gồm: Menu, HUD (điểm, xu, tim), và các màn hình state (Title, Game Over,...)
class UI
{
public:
    UI();
    void loadAssets(); // Tải font, hình menu, hình tim
    void setupTexts(); // Cài đặt vị trí, kích cỡ cho các đối tượng sf::Text

    // Reset HUD (điểm, xu, tim) về ban đầu
    void reset(int initialLives);

    // Cập nhật text điểm/xu (được gọi mỗi frame khi PLAYING)
    void update(int score, int coins);

    // Cập nhật lại số tim (chỉ gọi khi mất/thêm mạng)
    void updateHearts(int lives);

    // Chuẩn bị text cho màn hình điểm (chỉ gọi 1 lần khi GAME_OVER)
    void setupScoreScreen(const std::vector<int>& highScores, int finalScore);

    // Hàm vẽ chính, sẽ tự quyết định vẽ gì dựa trên trạng thái game (currentState)
    void render(sf::RenderTarget& target, GameState currentState);

private:
    // Hàm tiện ích nội bộ để tạo sprite tim
    sf::Sprite createHeartSprite(float x, float y);

    // --- Biến thành viên ---
    // Assets
    sf::Font mFont; // Font chữ (dùng chung cho mọi text)
    sf::Texture mHeartTexture; // Hình trái tim
    sf::Texture mMenuTexture; // Hình nền menu

    // Game Objects
    sf::Sprite mMenuSprite; // Sprite hình nền menu
    std::vector<sf::Sprite> mHearts; // Danh sách các sprite tim (HUD)

    // UI Texts (tất cả các chữ trong game)
    sf::Text mScoreText, mCoinsText, // HUD
        mLetsGoText, mGameOverText, mTitleText, // Màn hình state
        mFinalScoreText, mRankText, mLeaderboardText, mPressEnterText; // Màn hình điểm
};