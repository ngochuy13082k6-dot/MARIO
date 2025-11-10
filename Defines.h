#pragma once // Đảm bảo file chỉ được include một lần, tránh lỗi biên dịch

// === CÁC THƯ VIỆN CHUNG ===
// (Đây là nơi tập trung các thư viện SFML và C++ chuẩn)
#include <SFML/Graphics.hpp> // Thư viện đồ họa chính (vẽ hình, cửa sổ)
#include <SFML/Audio.hpp>    // Thư viện âm thanh
#include <iostream>          // Dùng cho std::cerr (in lỗi ra console)
#include <vector>            // Dùng cho std::vector (để lưu danh sách, vd: list platform, list item)
#include <string>            // Dùng cho std::string (vd: tên file)
#include <sstream>           // Dùng cho std::stringstream (dùng để xây dựng chuỗi text, vd: "Score: 100")
#include <fstream>           // Dùng cho std::ifstream, std::ofstream (đọc/ghi file điểm cao)
#include <algorithm>         // Dùng cho std::min, std::max, std::sort (sắp xếp điểm cao)
#include <cmath>             // Dùng cho std::sqrt (tính căn bậc 2, dùng cho logic item bay)
#include <cstdlib>           // Dùng cho rand() (tạo số ngẫu nhiên)
#include <ctime>             // Dùng cho time() (khởi tạo seed ngẫu nhiên)

// === CÁC HẰNG SỐ TOÀN CỤC ===
// (Các hằng số giúp bạn dễ dàng thay đổi game mà không cần sửa code logic)

// --- Cửa sổ & Camera ---
const float WINDOW_WIDTH = 1280.f; // Chiều rộng cửa sổ (và camera)
const float WINDOW_HEIGHT = 720.f; // Chiều cao cửa sổ (và camera)
const float GAME_WIDTH = WINDOW_WIDTH * 2; // Tổng chiều rộng của thế giới game (rộng 2 màn hình)
const float GAME_HEIGHT = WINDOW_HEIGHT; // Tổng chiều cao của thế giới game

// --- Vật lý (Physics) ---
const float GRAVITY = 0.6f; // Lực hút (pixel/frame^2)
const float MOVE_SPEED = 8.0f; // Tốc độ di chuyển ngang
const float JUMP_POWER = -13.0f; // Lực nhảy (số âm vì trục Y hướng xuống)
const float HOMING_SPEED = 6.0f; // Tốc độ item (nấm, xu) bay về phía người chơi
const float PLAYER_HIT_DURATION = 0.5f; // Thời gian (giây) player nháy đỏ (bất tử)
const float PLAYER_POWERUP_DURATION = 1.0f; // Thời gian (giây) player nháy xanh (ăn nấm)

// --- Animation Player (Sprite Sheet) ---
// (Các hằng số này xác định cách cắt tấm ảnh animation của Mario)
const int IDLE_FRAME_INDEX = 0; // Index của frame đứng yên
const int FIRST_RUN_FRAME_INDEX = 1; // Index frame bắt đầu chạy
const int LAST_RUN_FRAME_INDEX = 3; // Index frame kết thúc chạy
const float FRAME_WIDTH = 307.f; // Chiều rộng của 1 frame trong tấm ảnh
const float FRAME_HEIGHT = 402.f; // Chiều cao của 1 frame trong tấm ảnh
const float ANIM_SPEED = 0.05f; // Thời gian (giây) giữa 2 frame chạy

// --- Kích thước & Tỷ lệ (Scales) ---
const float COIN_SCALE = 0.05f;
const float BOX_SCALE = 0.01f;
const float MUSHROOM_SCALE = 0.08f;
const float FIRE_FLOWER_SCALE = 0.08f;
const float HEART_SCALE = 0.05f;
const float HEART_SPACING = 5.f; // Khoảng cách ngang giữa các hình trái tim
const float ADJUSTMENT_Y = 25.f; // Độ lệch (pixel) để hình Mario "ngồi" vừa vặn trên hitbox

// --- File ---
const std::string HIGHSCORE_FILE = "highscores.txt"; // Tên file lưu điểm cao

// === CÁC ENUM VÀ STRUCT DỮ LIỆU ===
// (Các kiểu dữ liệu tùy chỉnh của game)

// Các trạng thái của game (State Machine)
enum GameState {
    MENU,         // Màn hình Menu chính
    TITLE_SCREEN, // Màn hình "Super Mario PTIT"
    STARTING,     // Màn hình "LET'S GO!"
    PLAYING,      // Đang trong màn chơi
    GAME_OVER,    // Màn hình "GAME OVER"
    SCORE_SCREEN  // Màn hình hiển thị điểm và xếp hạng
};

// Các loại phần thưởng có thể có
enum RewardType {
    COIN,
    MUSHROOM,
    FIRE_FLOWER
};

// Cấu trúc cho Hộp bí ẩn (Mystery Box)
struct MysteryBox {
    sf::Sprite sprite; // Hình ảnh của hộp
    RewardType reward = COIN; // Loại quà chứa bên trong (khởi tạo mặc định là COIN)
};

// Cấu trúc cho Phần thưởng (khi bay ra từ hộp)
struct Reward {
    sf::Sprite sprite; // Hình ảnh (xu, nấm, hoặc hoa)
    RewardType type = COIN;   // Loại phần thưởng (khởi tạo mặc định là COIN)
    sf::Vector2f velocity; // Vận tốc di chuyển (dùng cho logic 'homing')
    bool homing = true;    // 'true' nghĩa là sẽ tự động bay về phía người chơi
};