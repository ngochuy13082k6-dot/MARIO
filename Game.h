#pragma once
#include "Defines.h"
#include "Player.h"
#include "Item.h"
#include "Map.h"
#include "UI.h"
#include "Enemy.h" // Thêm Enemy

class Game
{
public:
    Game();
    void run();
private:
    void processEvents();
    void update(float deltaTime);
    void render();
    void updatePlaying(float deltaTime);
    void resetGame(bool resetScoreAndCoins);
    void updateCamera();
    void loadHighScores();
    void saveHighScores();

    // Logic Quản lý Kẻ Thù
    void spawnEnemies();
    void updateEnemies(float deltaTime);

    sf::RenderWindow mWindow;
    GameState mCurrentState;
    int mCurrentLevel = 1;
    const int MAX_LEVELS = 2;
    sf::Music mBackgroundMusic;
    sf::SoundBuffer mGameOverBuffer;
    sf::Sound mGameOverSound;
    Player mPlayer;
    Item mItem;
    Map mMap;
    UI mUI;
    sf::View mCamera;
    sf::View mCloudView;
    float mStartingTimer;
    float mGameOverTimer;
    float mTitleTimer;
    float mVictoryTimer; // Timer màn hình thắng
    std::vector<int> mHighScores;

    // Các biến Quản lý Game
    std::vector<Enemy> mEnemies; // Vector lưu trữ Enemy
    sf::Texture mGoombaTexture; // Texture dùng chung
    int mVolume; // Biến lưu trữ âm lượng hiện tại
};