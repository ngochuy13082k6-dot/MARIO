#include "UI.h"
#include <sstream> 
#include <algorithm> 
#include <iostream> 

// Định nghĩa màu sắc cho UI: Vàng Neon (Tương phản tốt trên nền tối)
const sf::Color NEON_YELLOW = sf::Color(255, 255, 50);

sf::Sprite UI::createHeartSprite(float x, float y) {
    sf::Sprite heart(mHeartTexture);
    heart.setScale(HEART_SCALE, HEART_SCALE);
    heart.setPosition(x, y);
    return heart;
}
UI::UI() {}
void UI::loadAssets()
{
    if (!mFont.loadFromFile("Assets/PressStart2P-Regular.ttf")) { /* Xử lý lỗi */ }
    if (!mMenuTexture.loadFromFile("Assets/menu_background.jpg")) { /* Xử lý lỗi */ }
    mMenuSprite.setTexture(mMenuTexture);
    mMenuSprite.setScale(WINDOW_WIDTH / mMenuTexture.getSize().x, WINDOW_HEIGHT / mMenuTexture.getSize().y);
    if (!mHeartTexture.loadFromFile("Assets/heart.png")) { /* Xử lý lỗi */ }
}

void UI::setupTexts()
{
    // --- KHỞI TẠO HUD VÀ SCREEN TEXT ---
    mScoreText.setFont(mFont);
    mScoreText.setCharacterSize(24);
    mScoreText.setFillColor(sf::Color::White);
    mScoreText.setPosition(15.f, 15.f);
    mScoreText.setString("Score: 0");

    mCoinsText.setFont(mFont);
    mCoinsText.setCharacterSize(24);
    mCoinsText.setFillColor(sf::Color::White);
    mCoinsText.setPosition(15.f, mScoreText.getPosition().y + mScoreText.getGlobalBounds().height + 10.f);
    mCoinsText.setString("Coins: 0");

    // Màn hình Let's Go
    mLetsGoText.setFont(mFont);
    mLetsGoText.setString("LET'S GO!");
    mLetsGoText.setCharacterSize(60);
    mLetsGoText.setFillColor(sf::Color::White);
    sf::FloatRect letsGoBounds = mLetsGoText.getLocalBounds();
    mLetsGoText.setOrigin(letsGoBounds.left + letsGoBounds.width / 2.f, letsGoBounds.top + letsGoBounds.height / 2.f);
    mLetsGoText.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);

    // Màn hình Game Over
    mGameOverText.setFont(mFont);
    mGameOverText.setString("GAME OVER");
    mGameOverText.setCharacterSize(60);
    mGameOverText.setFillColor(sf::Color::White);
    sf::FloatRect gameOverBounds = mGameOverText.getLocalBounds();
    mGameOverText.setOrigin(gameOverBounds.left + gameOverBounds.width / 2.f, gameOverBounds.top + gameOverBounds.height / 2.f);
    mGameOverText.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);

    // Tiêu đề Game (Menu)
    mTitleText.setFont(mFont);
    mTitleText.setString("Super Mario PTIT");
    mTitleText.setCharacterSize(60);
    mTitleText.setFillColor(sf::Color::White);
    sf::FloatRect titleBounds = mTitleText.getLocalBounds();
    mTitleText.setOrigin(titleBounds.left + titleBounds.width / 2.f, titleBounds.top + titleBounds.height / 2.f);
    mTitleText.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f - 150.f);

    // ... (Khởi tạo các Text khác như Scoreboard, Press Enter) ...
    mFinalScoreText.setFont(mFont); mFinalScoreText.setCharacterSize(40); mFinalScoreText.setFillColor(sf::Color::White);
    mRankText.setFont(mFont); mRankText.setCharacterSize(30); mRankText.setFillColor(sf::Color::Cyan);
    mLeaderboardText.setFont(mFont); mLeaderboardText.setCharacterSize(20); mLeaderboardText.setFillColor(sf::Color::White);
    mPressEnterText.setFont(mFont); mPressEnterText.setString("Press ENTER to return to Menu"); mPressEnterText.setCharacterSize(20); mPressEnterText.setFillColor(sf::Color::White);
    sf::FloatRect pressEnterBounds = mPressEnterText.getLocalBounds(); mPressEnterText.setOrigin(pressEnterBounds.left + pressEnterBounds.width / 2.f, pressEnterBounds.top + pressEnterBounds.height / 2.f); mPressEnterText.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT - 100.f);

    // --- MENU BUTTONS (Size 50, Màu NEON_YELLOW) ---
    // 1. Start Game
    mStartButton.setFont(mFont);
    mStartButton.setString("START GAME");
    mStartButton.setCharacterSize(50);
    mStartButton.setFillColor(NEON_YELLOW);
    sf::FloatRect startBounds = mStartButton.getLocalBounds();
    mStartButton.setOrigin(startBounds.left + startBounds.width / 2.f, startBounds.top + startBounds.height / 2.f);
    mStartButton.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f - 100.f);

    // 2. High Scores
    mHighScoresButton.setFont(mFont);
    mHighScoresButton.setString("HIGH SCORES");
    mHighScoresButton.setCharacterSize(50);
    mHighScoresButton.setFillColor(NEON_YELLOW);
    sf::FloatRect highScoresBounds = mHighScoresButton.getLocalBounds();
    mHighScoresButton.setOrigin(highScoresBounds.left + highScoresBounds.width / 2.f, highScoresBounds.top + highScoresBounds.height / 2.f);
    mHighScoresButton.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f - 20.f);

    // 3. Volume
    mVolumeButton.setFont(mFont);
    mVolumeButton.setString("VOLUME: 50");
    mVolumeButton.setCharacterSize(50);
    mVolumeButton.setFillColor(NEON_YELLOW);
    sf::FloatRect volBounds = mVolumeButton.getLocalBounds();
    mVolumeButton.setOrigin(volBounds.left + volBounds.width / 2.f, volBounds.top + volBounds.height / 2.f);
    mVolumeButton.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f + 60.f);

    // 4. Exit
    mExitButton.setFont(mFont);
    mExitButton.setString("EXIT");
    mExitButton.setCharacterSize(50);
    mExitButton.setFillColor(NEON_YELLOW);
    sf::FloatRect exitBounds = mExitButton.getLocalBounds();
    mExitButton.setOrigin(exitBounds.left + exitBounds.width / 2.f, exitBounds.top + exitBounds.height / 2.f);
    mExitButton.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f + 140.f);

    mVictoryText.setFont(mFont);
    mVictoryText.setCharacterSize(40);
    mVictoryText.setFillColor(sf::Color::White);

    // --- Pause Menu Setup ---
    mPauseOverlay.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    mPauseOverlay.setFillColor(sf::Color(0, 0, 0, 150)); // Nền tối 60%
    mPauseText.setFont(mFont);
    mPauseText.setString("PAUSED");
    mPauseText.setCharacterSize(60);
    mPauseText.setFillColor(sf::Color::White);
    sf::FloatRect pauseBounds = mPauseText.getLocalBounds();
    mPauseText.setOrigin(pauseBounds.left + pauseBounds.width / 2.f, pauseBounds.top + pauseBounds.height / 2.f);
    mPauseText.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f - 100.f);
    mResumeButton.setFont(mFont);
    mResumeButton.setString("RESUME");
    mResumeButton.setCharacterSize(40);
    mResumeButton.setFillColor(sf::Color::White);
    sf::FloatRect resumeBounds = mResumeButton.getLocalBounds();
    mResumeButton.setOrigin(resumeBounds.left + resumeBounds.width / 2.f, resumeBounds.top + resumeBounds.height / 2.f);
    mResumeButton.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);
    mMenuButton.setFont(mFont);
    mMenuButton.setString("MENU");
    mMenuButton.setCharacterSize(40);
    mMenuButton.setFillColor(sf::Color::White);
    sf::FloatRect menuBtnBounds = mMenuButton.getLocalBounds();
    mMenuButton.setOrigin(menuBtnBounds.left + menuBtnBounds.width / 2.f, menuBtnBounds.top + menuBtnBounds.height / 2.f);
    mMenuButton.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f + 80.f);
}

void UI::reset(int initialLives, int score, int coins) {
    mScoreText.setString("Score: " + std::to_string(score));
    mCoinsText.setString("Coins: " + std::to_string(coins));
    updateHearts(initialLives);
}
void UI::update(int score, int coins) {
    mScoreText.setString("Score: " + std::to_string(score));
    mCoinsText.setString("Coins: " + std::to_string(coins));
}
void UI::updateHearts(int lives) {
    mHearts.clear();
    float heartY = mCoinsText.getPosition().y + mCoinsText.getGlobalBounds().height + 10.f;
    if (mHeartTexture.getSize().x > 0) {
        for (int i = 0; i < lives; ++i) {
            float heartX = 15.f + i * (mHeartTexture.getSize().x * HEART_SCALE + HEART_SPACING);
            mHearts.push_back(createHeartSprite(heartX, heartY));
        }
    }
}

void UI::setupScoreScreen(const std::vector<int>& highScores, int finalScore) {
    int playerRank = -1; int currentRank = 0; int lastScore = -1;
    for (size_t i = 0; i < highScores.size(); ++i) {
        if (highScores[i] != lastScore) {
            currentRank++;
            lastScore = highScores[i];
        }
        if (highScores[i] == finalScore && playerRank == -1) {
            playerRank = currentRank;
        }
    }
    mFinalScoreText.setString("Score: " + std::to_string(finalScore));
    sf::FloatRect finalBounds = mFinalScoreText.getLocalBounds();
    mFinalScoreText.setOrigin(finalBounds.left + finalBounds.width / 2.f, finalBounds.top + finalBounds.height / 2.f);
    mFinalScoreText.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f - 200.f);

    if (playerRank != -1) {
        mRankText.setString("Your Rank: " + std::to_string(playerRank));
    }
    else {
        mRankText.setString("Not in Top 10");
    }
    sf::FloatRect rankBounds = mRankText.getLocalBounds();
    mRankText.setOrigin(rankBounds.left + rankBounds.width / 2.f, rankBounds.top + rankBounds.height / 2.f);
    mRankText.setPosition(WINDOW_WIDTH / 2.f, mFinalScoreText.getPosition().y + 80.f);

    std::stringstream ss;
    ss << "TOP 5 SCORES:\n\n";
    int scoresToShow = std::min(static_cast<int>(highScores.size()), 5);
    currentRank = 0; lastScore = -1;
    for (int i = 0; i < scoresToShow; ++i) {
        if (highScores[i] != lastScore) {
            currentRank++;
            lastScore = highScores[i];
        }
        ss << currentRank << ". " << highScores[i] << "\n\n";
    }
    mLeaderboardText.setString(ss.str());
    sf::FloatRect leaderBounds = mLeaderboardText.getLocalBounds();
    mLeaderboardText.setOrigin(leaderBounds.left + leaderBounds.width / 2.f, leaderBounds.top);
    mLeaderboardText.setPosition(WINDOW_WIDTH / 2.f, mRankText.getPosition().y + 120.f);
}

// --- HÀM RENDER (VẼ LÊN MÀN HÌNH) ---
void UI::render(sf::RenderTarget& target, GameState currentState) {
    target.setView(target.getDefaultView()); // Dùng View tĩnh cho UI
    switch (currentState) {
    case MENU:
        target.clear();
        target.draw(mMenuSprite);
        target.draw(mStartButton);
        target.draw(mHighScoresButton);
        target.draw(mVolumeButton);
        target.draw(mExitButton);
        break;
    case TITLE_SCREEN:
        target.clear(sf::Color::Black);
        target.draw(mTitleText);
        break;
    case STARTING:
        target.clear(sf::Color::Black);
        target.draw(mLetsGoText);
        break;
    case PLAYING:
        target.draw(mScoreText);
        target.draw(mCoinsText);
        for (const auto& heart : mHearts) {
            target.draw(heart);
        }
        break;
    case PAUSED: // Pause Menu
        target.draw(mPauseOverlay);
        target.draw(mPauseText);
        target.draw(mResumeButton);
        target.draw(mMenuButton);
        break;
    case VICTORY:
        target.clear(sf::Color::Black);
        target.draw(mVictoryText);
        break;
    case GAME_OVER:
        target.clear(sf::Color::Black);
        target.draw(mGameOverText);
        break;
    case SCORE_SCREEN:
        target.clear(sf::Color::Black);
        target.draw(mFinalScoreText);
        target.draw(mRankText);
        target.draw(mLeaderboardText);
        target.draw(mPressEnterText);
        break;
    }
}

// --- GETTERS CHO BUTTONS ---
sf::FloatRect UI::getStartButtonBounds() const { return mStartButton.getGlobalBounds(); }
sf::FloatRect UI::getExitButtonBounds() const { return mExitButton.getGlobalBounds(); }
sf::FloatRect UI::getHighScoresButtonBounds() const { return mHighScoresButton.getGlobalBounds(); }
sf::FloatRect UI::getVolumeButtonBounds() const { return mVolumeButton.getGlobalBounds(); }
sf::FloatRect UI::getResumeButtonBounds() const { return mResumeButton.getGlobalBounds(); }
sf::FloatRect UI::getMenuButtonBounds() const { return mMenuButton.getGlobalBounds(); }

// --- LOGIC HOVER ---
void UI::updateMenuHover(const sf::Vector2f& mousePos) {
    // Reset về màu mặc định (NEON_YELLOW)
    mStartButton.setFillColor(NEON_YELLOW);
    mHighScoresButton.setFillColor(NEON_YELLOW);
    mExitButton.setFillColor(NEON_YELLOW);
    mVolumeButton.setFillColor(NEON_YELLOW);

    // Đổi sang WHITE khi hover
    if (mStartButton.getGlobalBounds().contains(mousePos)) {
        mStartButton.setFillColor(sf::Color::White);
    }
    else if (mHighScoresButton.getGlobalBounds().contains(mousePos)) {
        mHighScoresButton.setFillColor(sf::Color::White);
    }
    else if (mVolumeButton.getGlobalBounds().contains(mousePos)) {
        mVolumeButton.setFillColor(sf::Color::White);
    }
    else if (mExitButton.getGlobalBounds().contains(mousePos)) {
        mExitButton.setFillColor(sf::Color::White);
    }
}

void UI::updateVolumeDisplay(int volume)
{
    // Cập nhật text hiển thị âm lượng và căn giữa lại
    mVolumeButton.setString("VOLUME: " + std::to_string(volume));
    sf::FloatRect volBounds = mVolumeButton.getLocalBounds();
    mVolumeButton.setOrigin(volBounds.left + volBounds.width / 2.f, volBounds.top + volBounds.height / 2.f);
    mVolumeButton.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f + 60.f);
}

void UI::setupVictoryScreen(int levelWon, const int MAX_LEVELS) {
    // Thiết lập thông điệp chiến thắng
    if (levelWon < MAX_LEVELS) {
        mVictoryText.setString("Well Done!");
        mVictoryText.setCharacterSize(70);
    }
    else {
        mVictoryText.setString("Great Job!");
        mVictoryText.setCharacterSize(70);
    } // Màn cuối

    sf::FloatRect bounds = mVictoryText.getLocalBounds();
    mVictoryText.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    mVictoryText.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);
}

void UI::updatePauseHover(const sf::Vector2f& mousePos) {
    // Logic hover cho Menu Pause (Nút màu Trắng -> Hover màu Vàng)
    mResumeButton.setFillColor(sf::Color::White);
    mMenuButton.setFillColor(sf::Color::White);
    if (mResumeButton.getGlobalBounds().contains(mousePos)) {
        mResumeButton.setFillColor(sf::Color::Yellow);
    }
    else if (mMenuButton.getGlobalBounds().contains(mousePos)) {
        mMenuButton.setFillColor(sf::Color::Yellow);
    }
}