#include "Game.h"
#include <ctime> 
#include <fstream> 
#include <iostream> 
#include <algorithm> 
#include <vector>
#include <random> 

Game::Game()
    : mWindow(sf::VideoMode(static_cast<unsigned int>(WINDOW_WIDTH), static_cast<unsigned int>(WINDOW_HEIGHT)), "Mario Game - OOP!")
    , mCurrentState(MENU)
    , mCurrentLevel(1)
    , mStartingTimer(2.0f)
    , mGameOverTimer(5.0f)
    , mTitleTimer(5.0f)
    , mVictoryTimer(5.0f)
    , mVolume(50)
{
    srand(static_cast<unsigned int>(time(0)));
    mWindow.setFramerateLimit(60);

    if (!mBackgroundMusic.openFromFile("Assets/music.ogg")) { /*...*/ }
    mBackgroundMusic.setLoop(true);
    mBackgroundMusic.setVolume(static_cast<float>(mVolume));
    mBackgroundMusic.play();

    if (!mGameOverBuffer.loadFromFile("Assets/gameover.wav")) { /*...*/ }
    mGameOverSound.setBuffer(mGameOverBuffer);

    mCamera.setSize(WINDOW_WIDTH, WINDOW_HEIGHT); mCloudView.setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    mPlayer.loadAssets(); mItem.loadAssets(); mMap.loadAssets(); mUI.loadAssets(); mUI.setupTexts();

    if (!mGoombaTexture.loadFromFile("Assets/goomba.png")) {
        std::cerr << "Loi tai Assets/goomba.png\n";
    }

    mUI.updateVolumeDisplay(mVolume);
    loadHighScores();
}

void Game::run()
{
    sf::Clock clock;
    while (mWindow.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();
        if (deltaTime > 1.f / 20.f) { deltaTime = 1.f / 20.f; }

        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents()
{
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        if (event.type == sf::Event::Closed) { mWindow.close(); }
        switch (mCurrentState)
        {
        case MENU: // Xử lý Menu chính (Click chuột)
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2f mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));
                    if (mUI.getStartButtonBounds().contains(mousePos)) { mCurrentState = TITLE_SCREEN; mTitleTimer = 5.0f; if (mBackgroundMusic.getStatus() != sf::Music::Playing) { mBackgroundMusic.play(); } mCurrentLevel = 1; }
                    if (mUI.getHighScoresButtonBounds().contains(mousePos)) { mUI.setupScoreScreen(mHighScores, mPlayer.getScore()); mCurrentState = SCORE_SCREEN; }
                    if (mUI.getVolumeButtonBounds().contains(mousePos)) {
                        mVolume += 25;
                        if (mVolume > 100) mVolume = 0;
                        mUI.updateVolumeDisplay(mVolume);
                        mBackgroundMusic.setVolume(static_cast<float>(mVolume));
                    }
                    if (mUI.getExitButtonBounds().contains(mousePos)) { mWindow.close(); }
                }
            }
            break;
        case TITLE_SCREEN: if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) { mCurrentState = STARTING; mStartingTimer = 2.0f; } break;
        case PLAYING:
            mPlayer.handleEvent(event);
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) mCurrentState = PAUSED;
                else if (event.key.code == sf::Keyboard::Enter) { mCurrentState = GAME_OVER; mGameOverTimer = 5.0f; mBackgroundMusic.stop(); mGameOverSound.play(); }
            }
            break;
        case PAUSED:
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) mCurrentState = PLAYING;
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mousePixel = sf::Mouse::getPosition(mWindow);
                sf::Vector2f mousePos = mWindow.mapPixelToCoords(mousePixel, mWindow.getDefaultView());
                if (mUI.getResumeButtonBounds().contains(mousePos)) mCurrentState = PLAYING;
                else if (mUI.getMenuButtonBounds().contains(mousePos)) { mCurrentState = MENU; mCurrentLevel = 1; if (mBackgroundMusic.getStatus() != sf::Music::Playing) mBackgroundMusic.play(); }
            }
            break;
        case SCORE_SCREEN: if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) mCurrentState = MENU; break;
        case GAME_OVER: case STARTING: case VICTORY: break;
        }
    }
}

void Game::update(float deltaTime)
{
    switch (mCurrentState)
    {
    case TITLE_SCREEN: mTitleTimer -= deltaTime; if (mTitleTimer <= 0.f) { mCurrentState = STARTING; mStartingTimer = 2.0f; } break;
    case STARTING: mStartingTimer -= deltaTime; if (mStartingTimer <= 0.f) { mCurrentState = PLAYING; bool resetStats = (mCurrentLevel == 1); resetGame(resetStats); } break;
    case GAME_OVER:
        if (mBackgroundMusic.getStatus() == sf::Music::Playing) { mBackgroundMusic.stop(); mGameOverSound.play(); }
        mGameOverTimer -= deltaTime;
        if (mGameOverTimer <= 0.f) {
            mCurrentState = SCORE_SCREEN;
            mHighScores.push_back(mPlayer.getScore()); std::sort(mHighScores.rbegin(), mHighScores.rend());
            if (mHighScores.size() > 10) { mHighScores.resize(10); } saveHighScores();
            mUI.setupScoreScreen(mHighScores, mPlayer.getScore());
        }
        break;
    case VICTORY:
        mVictoryTimer -= deltaTime;
        if (mVictoryTimer <= 0.f)
        {
            if (mCurrentLevel < MAX_LEVELS) { mCurrentLevel++; mCurrentState = STARTING; mStartingTimer = 2.0f; }
            else {
                mCurrentState = SCORE_SCREEN;
                mHighScores.push_back(mPlayer.getScore()); std::sort(mHighScores.rbegin(), mHighScores.rend());
                if (mHighScores.size() > 10) { mHighScores.resize(10); } saveHighScores();
                mUI.setupScoreScreen(mHighScores, mPlayer.getScore());
            }
        }
        break;
    case PLAYING: updatePlaying(deltaTime); break;
    case MENU: { sf::Vector2f mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow)); mUI.updateMenuHover(mousePos); break; }
    case PAUSED: { sf::Vector2i mousePixel = sf::Mouse::getPosition(mWindow); sf::Vector2f mousePos = mWindow.mapPixelToCoords(mousePixel, mWindow.getDefaultView()); mUI.updatePauseHover(mousePos); break; }
    case SCORE_SCREEN: break;
    }
}

void Game::updatePlaying(float deltaTime)
{
    mPlayer.update(deltaTime, mMap.getPlatforms());
    mItem.update(deltaTime, mPlayer);
    mUI.update(mPlayer.getScore(), mPlayer.getCoins());
    mUI.updateHearts(mPlayer.getLives());
    updateCamera();
    updateEnemies(deltaTime);

    if (mPlayer.getGlobalBounds().intersects(mMap.getGoalBounds()))
    {
        mCurrentState = VICTORY;
        mVictoryTimer = 5.0f;
        mUI.setupVictoryScreen(mCurrentLevel, MAX_LEVELS);
        if (mCurrentLevel == MAX_LEVELS) { if (mBackgroundMusic.getStatus() == sf::Music::Playing) { mBackgroundMusic.stop(); } }
    }

    if (mPlayer.isDead() && mCurrentState == PLAYING) { mCurrentState = GAME_OVER; mGameOverTimer = 5.0f; }
}

void Game::render()
{
    if (mCurrentState == PLAYING) { mWindow.clear(sf::Color(92, 148, 252)); }
    if (mCurrentState == PLAYING) { mMap.renderParallax(mWindow, mCloudView, mCamera); }

    if (mCurrentState == PLAYING || mCurrentState == PAUSED)
    {
        mWindow.setView(mCamera);
        mMap.render(mWindow);
        mItem.render(mWindow);
        for (auto& enemy : mEnemies) { enemy.render(mWindow); }
        mPlayer.render(mWindow);
    }

    mUI.render(mWindow, mCurrentState);
    mWindow.display();
}

void Game::resetGame(bool resetScoreAndCoins)
{
    mPlayer.reset(resetScoreAndCoins);
    mMap.reset(mCurrentLevel);
    mItem.reset(mCurrentLevel, mMap.getPlatforms(), mMap.getGoalBounds());
    mUI.reset(mPlayer.getLives(), mPlayer.getScore(), mPlayer.getCoins());
    spawnEnemies();
}

void Game::updateCamera()
{
    float cameraX = mPlayer.getCenter().x;
    float cameraY = mPlayer.getCenter().y;
    float cameraYCenterMin = WINDOW_HEIGHT / 2.f;
    float cameraYPosMin = GAME_HEIGHT - cameraYCenterMin;

    if (GAME_HEIGHT <= WINDOW_HEIGHT) { cameraY = GAME_HEIGHT / 2.f; }
    else { if (cameraY > cameraYPosMin) cameraY = cameraYPosMin; }

    float cameraXCenterMin = WINDOW_WIDTH / 2.f;
    float cameraXPosMin = cameraXCenterMin;
    float cameraXMax = GAME_WIDTH - cameraXCenterMin;
    if (cameraX < cameraXPosMin) cameraX = cameraXPosMin;
    if (cameraX > cameraXMax) cameraX = cameraXMax;
    mCamera.setCenter(cameraX, cameraY);
}

void Game::spawnEnemies()
{
    mEnemies.clear();

    int enemyCount = (mCurrentLevel == 1) ? 10 : 12; // Số lượng Goomba theo Level
    const std::vector<sf::RectangleShape>& platforms = mMap.getPlatforms();

    // 1. Lọc các platform hợp lệ (ngoài vùng an toàn 200px)
    std::vector<int> validPlatformIndices;
    for (size_t i = 0; i < platforms.size(); ++i)
    {
        if (platforms[i].getPosition().x < 200.f) continue; // Vùng an toàn 200px
        validPlatformIndices.push_back(i);
    }

    std::random_device rd; std::default_random_engine g(rd());
    std::shuffle(validPlatformIndices.begin(), validPlatformIndices.end(), g);

    int count = 0;
    for (int index : validPlatformIndices)
    {
        if (count >= enemyCount) break;

        const sf::RectangleShape& plat = platforms[index];

        float x = plat.getPosition().x + (plat.getSize().x / 2.f);
        float y = plat.getPosition().y - 20.f;

        mEnemies.emplace_back(x, y, mGoombaTexture); // Truyền texture dùng chung
        count++;
    }
}

// --- LOGIC VA CHẠM ENEMY-PLAYER ---
void Game::updateEnemies(float deltaTime)
{
    for (auto it = mEnemies.begin(); it != mEnemies.end(); )
    {
        it->update(deltaTime);
        it->checkCollisionWithPlatforms(mMap.getPlatforms());

        if (!it->isDying())
        {
            sf::FloatRect playerBounds = mPlayer.getGlobalBounds();
            sf::FloatRect enemyBounds = it->getGlobalBounds();

            // Hitbox thu nhỏ của Goomba
            sf::FloatRect enemyHitbox = enemyBounds; enemyHitbox.left += 5.f; enemyHitbox.width -= 10.f;
            enemyHitbox.top += 5.f; enemyHitbox.height -= 5.f;

            if (playerBounds.intersects(enemyHitbox))
            {
                bool isFalling = mPlayer.getVelocity().y > 0;
                bool isAbove = playerBounds.top + playerBounds.height < enemyBounds.top + (enemyBounds.height * 0.5f);

                if (isAbove && isFalling) { // ĐẠP ĐẦU (Kill)
                    it->die();
                    mPlayer.addScore(30); // Cộng 30 điểm
                }
                else { // VA CHẠM NGANG (Damage)
                    // *** CHUẨN HÓA SÁT THƯƠNG: TRỪ 15 ĐIỂM (Chỉ trừ khi không bất tử) ***
                    if (!mPlayer.isHit()) {
                        mPlayer.addScore(-15);
                        mPlayer.takeDamage(); // Mất mạng và I-frames
                    }
                }
            }
        }

        if (it->isDead()) { it = mEnemies.erase(it); }
        else { ++it; } // Xóa kẻ thù đã chết hẳn
    }
}

void Game::loadHighScores()
{
    mHighScores.clear();
    std::ifstream inputFile;
    try {
        inputFile.open(HIGHSCORE_FILE);
        inputFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        int score;
        while (inputFile >> score) { mHighScores.push_back(score); }
        inputFile.close();
        std::sort(mHighScores.rbegin(), mHighScores.rend());
    }
    catch (const std::exception& e) { (void)e; if (inputFile.is_open()) { inputFile.close(); } }
}

void Game::saveHighScores()
{
    std::ofstream outputFile;
    try {
        outputFile.open(HIGHSCORE_FILE);
        outputFile.exceptions(std::ofstream::failbit | std::ofstream::badbit);
        for (int score : mHighScores) { outputFile << score << std::endl; }
        outputFile.close();
    }
    catch (const std::exception& e) {
        std::cerr << "Error saving high score file: " << e.what() << std::endl;
        if (outputFile.is_open()) { outputFile.close(); }
    }
}