#pragma once
#include "Defines.h"
#include <vector>
#include <string>

class UI
{
public:
    UI();
    void loadAssets();
    void setupTexts();
    void reset(int initialLives, int score, int coins);
    void update(int score, int coins);
    void updateHearts(int lives);

    void render(sf::RenderTarget& target, GameState currentState);

    // --- Getters cho Button (dùng trong Game.cpp để check click) ---
    sf::FloatRect getStartButtonBounds() const;
    sf::FloatRect getExitButtonBounds() const;
    sf::FloatRect getHighScoresButtonBounds() const;
    sf::FloatRect getVolumeButtonBounds() const;
    sf::FloatRect getResumeButtonBounds() const;
    sf::FloatRect getMenuButtonBounds() const;

    // --- Logic Menu/Pause ---
    void updateMenuHover(const sf::Vector2f& mousePos); // Hiệu ứng hover cho Menu chính
    void updatePauseHover(const sf::Vector2f& mousePos); // Hiệu ứng hover cho Menu Pause
    void setupScoreScreen(const std::vector<int>& highScores, int finalScore);
    void setupVictoryScreen(int currentLevel, int maxLevels);
    void updateVolumeDisplay(int volume); // Cập nhật hiển thị âm lượng

private:
    // ... (Các biến thành viên giữ nguyên) ...
    sf::Sprite createHeartSprite(float x, float y);
    sf::Font mFont;
    sf::Texture mHeartTexture;
    sf::Texture mMenuTexture;
    sf::Sprite mMenuSprite;
    sf::RectangleShape mPauseOverlay;
    std::vector<sf::Sprite> mHearts;

    sf::Text mScoreText, mCoinsText, mLetsGoText, mGameOverText, mFinalScoreText, mRankText, mLeaderboardText, mPressEnterText;
    sf::Text mTitleText;

    sf::Text mStartButton;
    sf::Text mExitButton;
    sf::Text mHighScoresButton;
    sf::Text mVolumeButton;

    sf::Text mPauseText;
    sf::Text mResumeButton;
    sf::Text mMenuButton;

    sf::Text mVictoryText;
};