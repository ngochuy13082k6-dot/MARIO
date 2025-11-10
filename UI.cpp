// *** DÒNG NÀY CỰC KỲ QUAN TRỌNG! ***
#include "UI.h"

// Hàm tiện ích (Từ file gốc) - Tạo 1 sprite tim
sf::Sprite UI::createHeartSprite(float x, float y) {
    sf::Sprite heart(mHeartTexture);
    heart.setScale(HEART_SCALE, HEART_SCALE);
    heart.setPosition(x, y);
    return heart;
}

UI::UI()
{
    // Hàm khởi tạo, để trống
}

// Tải assets cho UI
void UI::loadAssets()
{
    // --- TẢI FONT CHUNG --- (Từ main() cũ)
    if (!mFont.loadFromFile("Assets/PressStart2P-Regular.ttf")) {
        std::cerr << "Loi tai Assets/PressStart2P-Regular.ttf" << std::endl;
    }

    // --- ASSETS MENU --- (Từ main() cũ)
    if (!mMenuTexture.loadFromFile("Assets/menu_background.jpg")) {
        std::cerr << "Loi tai Assets/menu_background.jpg" << std::endl;
    }
    mMenuSprite.setTexture(mMenuTexture);
    // Scale hình nền cho vừa cửa sổ
    mMenuSprite.setScale(WINDOW_WIDTH / mMenuTexture.getSize().x, WINDOW_HEIGHT / mMenuTexture.getSize().y);

    // --- ASSET TIM --- (Từ main() cũ)
    if (!mHeartTexture.loadFromFile("Assets/heart.png")) { /* Van chay */ }
}

// Cài đặt tất cả các đối tượng sf::Text (vị trí, font, cỡ chữ)
void UI::setupTexts()
{
    // --- TEXT & UI (HUD) --- (Từ main() cũ)
    // (Đây là Điểm và Xu hiển thị khi chơi)
    mScoreText.setFont(mFont); mScoreText.setCharacterSize(24); mScoreText.setFillColor(sf::Color::White); mScoreText.setPosition(15.f, 15.f);
    mScoreText.setString("Score: 0");
    mCoinsText.setFont(mFont); mCoinsText.setCharacterSize(24); mCoinsText.setFillColor(sf::Color::White);
    mCoinsText.setPosition(15.f, mScoreText.getPosition().y + mScoreText.getGlobalBounds().height + 10.f);
    mCoinsText.setString("Coins: 0");

    // --- TEXT CHO CÁC MÀN HÌNH STATE --- (Từ main() cũ)
    // (Đây là các text hiển thị full màn hình)
    mLetsGoText.setFont(mFont); mLetsGoText.setString("LET'S GO!"); mLetsGoText.setCharacterSize(60); mLetsGoText.setFillColor(sf::Color::White);
    // setOrigin(center) để text được căn giữa
    sf::FloatRect letsGoBounds = mLetsGoText.getLocalBounds(); mLetsGoText.setOrigin(letsGoBounds.left + letsGoBounds.width / 2.f, letsGoBounds.top + letsGoBounds.height / 2.f); mLetsGoText.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);

    mGameOverText.setFont(mFont); mGameOverText.setString("GAME OVER"); mGameOverText.setCharacterSize(60); mGameOverText.setFillColor(sf::Color::White);
    sf::FloatRect gameOverBounds = mGameOverText.getLocalBounds(); mGameOverText.setOrigin(gameOverBounds.left + gameOverBounds.width / 2.f, gameOverBounds.top + gameOverBounds.height / 2.f); mGameOverText.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);

    mTitleText.setFont(mFont); mTitleText.setString("Super Mario PTIT"); mTitleText.setCharacterSize(60); mTitleText.setFillColor(sf::Color::White);
    sf::FloatRect titleBounds = mTitleText.getLocalBounds(); mTitleText.setOrigin(titleBounds.left + titleBounds.width / 2.f, titleBounds.top + titleBounds.height / 2.f);
    mTitleText.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);

    // --- TEXT MÀN HÌNH ĐIỂM --- (Từ main() cũ)
    mFinalScoreText.setFont(mFont); mFinalScoreText.setCharacterSize(40); mFinalScoreText.setFillColor(sf::Color::White);
    mRankText.setFont(mFont); mRankText.setCharacterSize(30); mRankText.setFillColor(sf::Color::Cyan);
    mLeaderboardText.setFont(mFont); mLeaderboardText.setCharacterSize(20); mLeaderboardText.setFillColor(sf::Color::White);
    mPressEnterText.setFont(mFont); mPressEnterText.setString("Press ENTER to return to Menu"); mPressEnterText.setCharacterSize(20); mPressEnterText.setFillColor(sf::Color::White);
    sf::FloatRect pressEnterBounds = mPressEnterText.getLocalBounds(); mPressEnterText.setOrigin(pressEnterBounds.left + pressEnterBounds.width / 2.f, pressEnterBounds.top + pressEnterBounds.height / 2.f); mPressEnterText.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT - 100.f);
}

// Reset HUD (điểm, xu, tim)
void UI::reset(int initialLives)
{
    // --- LOGIC TỪ resetGame() CŨ ---
    mScoreText.setString("Score: 0");
    mCoinsText.setString("Coins: 0");
    updateHearts(initialLives); // Vẽ lại số tim ban đầu
}

// Cập nhật text điểm và xu (gọi mỗi frame)
void UI::update(int score, int coins)
{
    // --- LOGIC TỪ updatePlaying() CŨ ---
    // (Chuyển số thành chuỗi)
    mScoreText.setString("Score: " + std::to_string(score));
    mCoinsText.setString("Coins: " + std::to_string(coins));
}

// Cập nhật lại số tim (vẽ lại từ đầu)
void UI::updateHearts(int lives)
{
    // --- LOGIC TỪ resetGame() / updatePlaying() CŨ ---
    mHearts.clear(); // Xóa hết tim cũ
    float heartY = mCoinsText.getPosition().y + mCoinsText.getGlobalBounds().height + 10.f; // Vị trí Y (dưới text xu)
    if (mHeartTexture.getSize().x > 0) {
        for (int i = 0; i < lives; ++i) { // Vẽ lại đúng số tim mới
            float heartX = 15.f + i * (mHeartTexture.getSize().x * HEART_SCALE + HEART_SPACING); // Vị trí X (xếp hàng)
            mHearts.push_back(createHeartSprite(heartX, heartY));
        }
    }
}

// Chuẩn bị text cho màn hình điểm
void UI::setupScoreScreen(const std::vector<int>& highScores, int finalScore)
{
    // --- LOGIC TỪ VÒNG LẶP GAME (case GAME_OVER) CŨ ---

    // (Tính toán xếp hạng - "Dense Rank")
    // (Dense Rank: 100, 100, 90 -> Hạng: 1, 1, 2)
    int playerRank = -1; // -1 nghĩa là không được xếp hạng
    int currentRank = 0;
    int lastScore = -1; // Điểm của hạng trước đó
    for (size_t i = 0; i < highScores.size(); ++i) {
        if (highScores[i] != lastScore) { // Nếu điểm khác
            currentRank++; // Tăng hạng
            lastScore = highScores[i]; // Cập nhật điểm
        }
        if (highScores[i] == finalScore && playerRank == -1) { // Tìm thấy điểm của người chơi
            playerRank = currentRank; // Gán hạng (chỉ 1 lần)
        }
    }

    // (Cài đặt nội dung và vị trí cho text)
    mFinalScoreText.setString("Score: " + std::to_string(finalScore));
    sf::FloatRect finalBounds = mFinalScoreText.getLocalBounds();
    mFinalScoreText.setOrigin(finalBounds.left + finalBounds.width / 2.f, finalBounds.top + finalBounds.height / 2.f);
    mFinalScoreText.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f - 200.f); // Đẩy lên cao

    if (playerRank != -1) { mRankText.setString("Your Rank: " + std::to_string(playerRank)); }
    else { mRankText.setString("Not in Top 10"); }
    sf::FloatRect rankBounds = mRankText.getLocalBounds();
    mRankText.setOrigin(rankBounds.left + rankBounds.width / 2.f, rankBounds.top + rankBounds.height / 2.f);
    mRankText.setPosition(WINDOW_WIDTH / 2.f, mFinalScoreText.getPosition().y + 80.f);

    // (Dùng stringstream để xây dựng chuỗi Top 5)
    std::stringstream ss;
    ss << "TOP 5 SCORES:\n\n";
    int scoresToShow = std::min(static_cast<int>(highScores.size()), 5); // Lấy tối đa 5 điểm
    currentRank = 0;
    lastScore = -1;
    for (int i = 0; i < scoresToShow; ++i) {
        if (highScores[i] != lastScore) {
            currentRank++;
            lastScore = highScores[i];
        }
        ss << currentRank << ". " << highScores[i] << "\n\n"; // Thêm vào chuỗi
    }
    mLeaderboardText.setString(ss.str()); // Gán chuỗi cho text
    sf::FloatRect leaderBounds = mLeaderboardText.getLocalBounds();
    mLeaderboardText.setOrigin(leaderBounds.left + leaderBounds.width / 2.f, leaderBounds.top);
    mLeaderboardText.setPosition(WINDOW_WIDTH / 2.f, mRankText.getPosition().y + 120.f);
}


// Vẽ UI (tùy theo trạng thái game)
void UI::render(sf::RenderTarget& target, GameState currentState)
{
    // --- LOGIC VẼ TỪ VÒNG LẶP GAME CŨ ---
    target.setView(target.getDefaultView()); // Luôn dùng camera mặc định (không di chuyển) cho UI

    // Vẽ đúng màn hình dựa trên trạng thái hiện tại
    switch (currentState)
    {
    case MENU:
        target.clear(); // Xóa màn hình
        target.draw(mMenuSprite); // Vẽ nền menu
        break;
    case TITLE_SCREEN:
        target.clear(sf::Color::Black); // Xóa màn hình đen
        target.draw(mTitleText); // Vẽ chữ title
        break;
    case STARTING:
        target.clear(sf::Color::Black);
        target.draw(mLetsGoText); // Vẽ chữ "LET'S GO!"
        break;
    case PLAYING:
        // Khi đang chơi, chỉ vẽ HUD (điểm, xu, tim)
        // (Không clear màn hình, vì Game::render đã clear màu trời)
        target.draw(mScoreText);
        target.draw(mCoinsText);
        for (const auto& heart : mHearts) { target.draw(heart); }
        break;
    case GAME_OVER:
        target.clear(sf::Color::Black);
        target.draw(mGameOverText); // Vẽ chữ "GAME OVER"
        break;
    case SCORE_SCREEN:
        target.clear(sf::Color::Black);
        target.draw(mFinalScoreText); // Vẽ điểm
        target.draw(mRankText);       // Vẽ rank
        target.draw(mLeaderboardText); // Vẽ top 5
        target.draw(mPressEnterText); // Vẽ hướng dẫn
        break;
    }
}