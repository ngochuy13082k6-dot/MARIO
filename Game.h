#pragma once
#include "Defines.h"
#include "Player.h"
#include "Item.h"
#include "Map.h"
#include "UI.h"

// Class Game là "nhạc trưởng" quản lý tất cả các đối tượng khác
// và chứa vòng lặp game chính
// (Mô hình "Composition": Game "có một" Player, "có một" Item,...)
class Game
{
public:
    Game(); // Hàm khởi tạo (Constructor): Tạo cửa sổ, tải tất cả tài nguyên
    void run(); // Bắt đầu vòng lặp game

private:
    // Ba hàm chính của vòng lặp game
    void processEvents(); // Xử lý input (đóng cửa sổ, nhấn phím thay đổi state)
    void update(float deltaTime); // Cập nhật logic (quản lý state, gọi update của đối tượng khác)
    void render(); // Vẽ mọi thứ (quản lý camera, gọi render của đối tượng khác)

    // Các hàm helper (hỗ trợ)
    void updatePlaying(float deltaTime); // Logic cập nhật cụ thể khi đang ở state PLAYING
    void resetGame(); // Reset tất cả các đối tượng (Player, Map, Item, UI)
    void updateCamera(); // Cập nhật camera đi theo Player

    // Hàm xử lý file điểm cao
    void loadHighScores();
    void saveHighScores();

    // --- Biến thành viên ---
    sf::RenderWindow mWindow; // Cửa sổ game
    GameState mCurrentState; // Trạng thái game hiện tại (MENU, PLAYING,...)

    // --- ÂM THANH GAME ---
    sf::Music mBackgroundMusic; // Dùng Music cho file dài (nhạc nền), stream từ file
    sf::SoundBuffer mGameOverBuffer; // Dùng Sound cho file ngắn (âm thanh)
    sf::Sound mGameOverSound;

    // --- Đối tượng game ---
    // (Game "sở hữu" các đối tượng này)
    Player mPlayer;
    Item mItem;
    Map mMap;
    UI mUI;

    // --- Views & Timers ---
    sf::View mCamera; // Camera chính (di chuyển theo Player)
    sf::View mCloudView; // Camera riêng cho mây (để tạo hiệu ứng parallax)
    float mStartingTimer; // Đếm ngược cho state STARTING
    float mGameOverTimer; // Đếm ngược cho state GAME_OVER
    float mTitleTimer;    // Đếm ngược cho state TITLE_SCREEN
    std::vector<int> mHighScores; // Danh sách điểm cao
};