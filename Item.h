#pragma once
#include "Defines.h"

// Khai báo trước class Player (Forward declaration)
// Giúp Item.h biết "Player" là một class mà không cần #include "Player.h"
// (Tránh lỗi include vòng tròn)
class Player;

// Class Item quản lý (coins, boxes, nấm, hoa)
class Item
{
public:
    Item();
    void loadAssets(); // Tải hình ảnh (texture) VÀ âm thanh cho tất cả item

    // Reset và sinh (spawn) tất cả item ra màn chơi
    // Cần biết 'platforms' ở đâu để tránh sinh item trong tường
    void reset(const std::vector<sf::RectangleShape>& platforms);

    // Cập nhật logic item (va chạm với Player, di chuyển item đang bay)
    // Cần tham chiếu '&' tới Player để có thể "tác động" (gọi hàm addScore, addCoin...)
    void update(float deltaTime, Player& player);

    // Vẽ tất cả item
    void render(sf::RenderTarget& target);

private:
    // --- Hàm tiện ích nội bộ ---
    // Tính toán vector đơn vị (để xác định hướng bay của item)
    sf::Vector2f normalize(const sf::Vector2f& source);

    // --- Các hàm nội bộ để sinh (spawn) item ---
    // (Chia hàm reset() thành các hàm con cho dễ đọc)
    void spawnCoins(const std::vector<sf::RectangleShape>& platforms);
    void spawnBoxes(const std::vector<sf::RectangleShape>& platforms);
    void spawnFireFlowers(const std::vector<sf::RectangleShape>& platforms, const sf::RectangleShape& ground);

    // --- Các hàm nội bộ để cập nhật ---
    // (Chia hàm update() thành các hàm con cho dễ đọc)
    void updateActiveRewards(float deltaTime, Player& player); // Cập nhật item (nấm, xu) đang bay về player
    void checkCollisions(Player& player); // Kiểm tra tất cả va chạm (player vs item)

    // --- Biến thành viên ---
    // Assets (hình ảnh gốc)
    sf::Texture mCoinTexture;
    sf::Texture mMysteryBoxTexture;
    sf::Texture mMushroomTexture;
    sf::Texture mFireFlowerTexture;

    // --- ÂM THANH ITEM ---
    sf::SoundBuffer mCoinBuffer; // "Khay" chứa file âm thanh xu
    sf::Sound mCoinSound;        // "Loa" phát âm thanh xu
    sf::SoundBuffer mPowerupBuffer;
    sf::Sound mPowerupSound;
    sf::SoundBuffer mDamageBuffer;
    sf::Sound mDamageSound;

    // Game Objects (Danh sách các item đang có trong màn chơi)
    std::vector<sf::Sprite> mCoins;
    std::vector<MysteryBox> mMysteryBoxes;
    std::vector<Reward> mActiveRewards; // Các item (nấm, xu) đã bay ra từ hộp
    std::vector<sf::Sprite> mGroundFireFlowers; // Hoa lửa nằm trên platform/mặt đất
};