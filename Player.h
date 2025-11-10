#pragma once
#include "Defines.h" // Nhúng file định nghĩa chung

// Class Player quản lý mọi thứ về người chơi
// Đây là "bản thiết kế" (blueprint) cho đối tượng Player.
// Nó khai báo Player "CÓ THỂ LÀM GÌ" (hàm public)
// và "CÓ GÌ BÊN TRONG" (biến private)
class Player
{
public: // Các hàm này có thể được gọi từ bên ngoài (vd: từ class Game)
    Player(); // Hàm khởi tạo (Constructor) - Được gọi khi 1 đối tượng Player được tạo ra

    // Hàm tải tài nguyên (hình ảnh, âm thanh) cho Player
    void loadAssets();

    // Hàm reset Player về trạng thái ban đầu (khi bắt đầu màn chơi mới)
    void reset();

    // Xử lý các sự kiện (vd: nhấn phím nhảy 1 lần)
    void handleEvent(sf::Event event);

    // Cập nhật logic của Player mỗi frame (di chuyển, trọng lực, va chạm platform)
    // 'deltaTime' là thời gian giữa các frame, giúp game mượt mà
    // 'platforms' là danh sách các bục để Player kiểm tra va chạm
    void update(float deltaTime, const std::vector<sf::RectangleShape>& platforms);

    // Vẽ Player ra cửa sổ game
    // 'target' thường chính là cửa sổ (window) mà ta muốn vẽ lên
    void render(sf::RenderTarget& target);

    // --- Các hàm Get (Getter) ---
    // (Cho phép các class khác "đọc" thông tin của Player mà không được sửa)
    // 'const' ở cuối đảm bảo hàm này không làm thay đổi giá trị của Player
    sf::FloatRect getGlobalBounds() const; // Lấy hộp va chạm (hitbox)
    sf::Vector2f getPosition() const;      // Lấy vị trí (góc trên bên trái hitbox)
    sf::Vector2f getCenter() const;        // Lấy vị trí trung tâm (cho item bay tới)
    bool isHit() const;                    // Trả về 'true' nếu Player đang trong trạng thái nháy đỏ (bất tử tạm thời)
    int getScore() const;                 // Lấy điểm số
    int getCoins() const;                 // Lấy số xu
    int getLives() const;                 // Lấy số mạng
    bool isDead() const;                   // Trả về 'true' nếu Player đã hết mạng

    // --- Các hàm Set (Setter) / Hành động ---
    // (Cho phép các class khác "ra lệnh" cho Player làm gì đó. VD: Item ra lệnh cho Player cộng điểm)
    void addScore(int points);             // Cộng điểm
    void addCoin(int amount);              // Cộng xu
    void addLife();                        // Cộng mạng
    void takeDamage();                     // Xử lý khi Player nhận sát thương (mất máu, nháy đỏ)
    void collectPowerup();                 // Xử lý khi Player ăn nấm (nháy xanh)
    void setOnGround(bool onGround);       // Đặt trạng thái đang đứng trên đất (dùng bởi hàm checkCollisions)
    void setGameOver();                    // Hàm này (hiện không dùng) có thể dùng để ép Player chết

private: // Các hàm và biến này chỉ được dùng "nội bộ" bên trong Player.cpp

    // --- Các hàm xử lý nội bộ (Private Helper) ---
    // (Chia nhỏ hàm update() cho dễ quản lý)
    void updateAnimation(float deltaTime, bool isMoving); // Cập nhật frame animation dựa trên trạng thái di chuyển
    void applyPhysics(float deltaTime);                   // Áp dụng trọng lực vào vận tốc
    void checkCollisions(const std::vector<sf::RectangleShape>& platforms, sf::Vector2f displacement); // Kiểm tra va chạm platform
    void updateEffects(float deltaTime);                  // Cập nhật thời gian nháy đỏ/nháy xanh

    // --- Biến thành viên (Member Variables) ---
    // (Lưu trữ toàn bộ trạng thái của Player)
    sf::Texture mTexture;        // Tấm ảnh gốc (sprite sheet)
    sf::Sprite mSprite;          // Sprite (hình ảnh) sẽ được vẽ, được cắt từ mTexture
    sf::RectangleShape mHitbox;  // Hộp va chạm (hình chữ nhật) dùng để tính toán vật lý

    sf::Vector2f mVelocity;      // Vận tốc hiện tại (x: ngang, y: dọc)
    int mScore;                  // Điểm số
    int mCoinsCollected;         // Số xu
    int mLives;                  // Số mạng

    bool mOnGround;              // 'true' nếu đang đứng trên đất (cho phép nhảy)
    bool mPlayerHit;             // 'true' nếu đang nháy đỏ
    float mPlayerHitTimer;       // Bộ đếm thời gian còn lại của hiệu ứng nháy đỏ
    bool mPlayerPowerup;         // 'true' nếu đang nháy xanh
    float mPlayerPowerupTimer;   // Bộ đếm thời gian còn lại của hiệu ứng nháy xanh
    bool mFacingRight;           // 'true' nếu đang quay mặt sang phải
    int mCurrentAnimationFrame;  // Frame animation hiện tại (index)
    float mAnimTimer;            // Bộ đếm thời gian để chuyển frame animation

    bool mIsDead;                // 'true' khi mLives <= 0

    // --- ÂM THANH PLAYER (Đã xóa âm thanh death) ---
    sf::SoundBuffer mJumpBuffer; // "Khay" chứa file âm thanh nhảy
    sf::Sound mJumpSound;        // "Loa" để phát âm thanh nhảy
};