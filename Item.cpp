// *** NHỮNG DÒNG NÀY CỰC KỲ QUAN TRỌNG! ***
#include "Item.h"   // Phải include file .h của chính nó
#include "Player.h" // Phải include file Player.h để biết các hàm của Player

Item::Item()
{
    // Hàm khởi tạo, để trống
}

// Hàm tiện ích (Từ file gốc) - Tính vector đơn vị
sf::Vector2f Item::normalize(const sf::Vector2f& source) {
    float length = std::sqrt((source.x * source.x) + (source.y * source.y));
    if (length != 0) {
        return sf::Vector2f(source.x / length, source.y / length);
    }
    else {
        return source;
    }
}

// Tải tất cả hình ảnh và âm thanh của item
void Item::loadAssets()
{
    // --- Tải Textures (Hình ảnh) ---
    if (!mCoinTexture.loadFromFile("Assets/coin.png")) { /* Van chay */ }
    if (!mMysteryBoxTexture.loadFromFile("Assets/mystery_box.png")) { /* Van chay */ }
    if (!mMushroomTexture.loadFromFile("Assets/mushroom.png")) { /* Van chay */ }
    if (!mFireFlowerTexture.loadFromFile("Assets/fire_flower.png")) { /* Van chay */ }

    // --- TẢI ÂM THANH (Đã giảm âm lượng) ---
    if (!mCoinBuffer.loadFromFile("Assets/coin.wav"))
    {
        std::cerr << "Loi tai Assets/coin.wav" << std::endl;
    }
    mCoinSound.setBuffer(mCoinBuffer);
    mCoinSound.setVolume(50.f); // <-- ĐÃ GIẢM ÂM LƯỢNG XU

    if (!mPowerupBuffer.loadFromFile("Assets/powerup.wav"))
    {
        std::cerr << "Loi tai Assets/powerup.wav" << std::endl;
    }
    mPowerupSound.setBuffer(mPowerupBuffer);
    mPowerupSound.setVolume(50.f); // <-- ĐÃ GIẢM ÂM LƯỢNG NẤM

    if (!mDamageBuffer.loadFromFile("Assets/damage.wav"))
    {
        std::cerr << "Loi tai Assets/damage.wav" << std::endl;
    }
    mDamageSound.setBuffer(mDamageBuffer);
    // (Âm lượng damage giữ nguyên 100%)
}

// Reset và sinh ra các item
void Item::reset(const std::vector<sf::RectangleShape>& platforms)
{
    // --- LOGIC TỪ resetGame() CŨ ---
    // Xóa hết item của màn chơi trước
    mCoins.clear();
    mMysteryBoxes.clear();
    mActiveRewards.clear();
    mGroundFireFlowers.clear();

    if (platforms.empty()) return; // Bảo vệ: Nếu chưa có platform thì không sinh

    // Gọi các hàm helper để sinh item
    spawnCoins(platforms);
    spawnBoxes(platforms);
    spawnFireFlowers(platforms, platforms[0]); // platforms[0] luôn là mặt đất
}

// Cập nhật logic item mỗi frame
void Item::update(float deltaTime, Player& player)
{
    // Cập nhật các item đang bay (nấm, xu từ hộp)
    updateActiveRewards(deltaTime, player);

    // Kiểm tra va chạm giữa player và tất cả item
    checkCollisions(player);
}

// Vẽ tất cả item ra màn hình
void Item::render(sf::RenderTarget& target)
{
    // --- LOGIC VẼ TỪ renderGame() CŨ ---
    // Vẽ theo thứ tự (để đảm bảo không bị che khuất sai)
    for (const auto& box : mMysteryBoxes) { target.draw(box.sprite); }
    for (const auto& coin : mCoins) { target.draw(coin); }
    for (const auto& reward : mActiveRewards) { target.draw(reward.sprite); }
    for (const auto& flower : mGroundFireFlowers) { target.draw(flower); }
}

// === CÁC HÀM PRIVATE (HELPER) ===

// Cập nhật các item (nấm, xu) đang bay về phía người chơi
void Item::updateActiveRewards(float deltaTime, Player& player)
{
    // --- CẬP NHẬT PHẦN THƯỞNG (BAY VÀO NGƯỜI) --- (Từ updatePlaying())
    sf::Vector2f playerCenter = player.getCenter(); // Lấy vị trí trung tâm player
    for (auto& reward : mActiveRewards) {
        if (reward.homing) { // Nếu item này được set là 'homing'
            sf::Vector2f direction = playerCenter - reward.sprite.getPosition(); // Tìm hướng
            reward.velocity = normalize(direction) * HOMING_SPEED; // Tính vận tốc bay
            reward.sprite.move(reward.velocity * deltaTime * 60.f); // Di chuyển
        }
    }
}

// Kiểm tra tất cả va chạm của Player với Item
void Item::checkCollisions(Player& player)
{
    sf::FloatRect playerBounds = player.getGlobalBounds(); // Lấy hitbox của player

    // --- COLLISION (Player vs Coins) --- (Từ updatePlaying())
    // (Dùng vòng lặp iterator (với .begin(), .end(), .erase())
    //  để có thể xóa item ngay trong lúc lặp)
    for (auto coinIter = mCoins.begin(); coinIter != mCoins.end(); ) {
        if (playerBounds.intersects(coinIter->getGlobalBounds())) {
            coinIter = mCoins.erase(coinIter); // Xóa đồng xu
            player.addCoin(1); // Tác động đến Player (Encapsulation)
            player.addScore(10);
            mCoinSound.play(); // Phát âm thanh xu (đã giảm)
        }
        else {
            ++coinIter; // Chỉ tăng iterator nếu không xóa
        }
    }

    // --- COLLISION (Player vs Mystery Boxes) --- (Từ updatePlaying())
    for (auto boxIter = mMysteryBoxes.begin(); boxIter != mMysteryBoxes.end(); ) {
        if (playerBounds.intersects(boxIter->sprite.getGlobalBounds())) {
            player.addScore(20); // Cộng điểm khi đụng hộp
            sf::FloatRect boxBounds = boxIter->sprite.getGlobalBounds();
            // Vị trí quà bay ra (từ tâm hộp)
            sf::Vector2f rewardPos = { boxBounds.left + boxBounds.width / 2.f, boxBounds.top + boxBounds.height / 2.f };

            // Logic sinh quà từ hộp
            if (boxIter->reward == COIN) {
                // ... (Logic sinh 1-3 xu) ...
                int numCoinsToSpawn = 1;
                int coinAmountRand = rand() % 100;
                if (coinAmountRand < 10) numCoinsToSpawn = 3;
                else if (coinAmountRand < 30) numCoinsToSpawn = 2;
                else numCoinsToSpawn = 1;
                for (int c = 0; c < numCoinsToSpawn; ++c) {
                    Reward newCoinReward; // Tạo 1 Reward mới
                    newCoinReward.homing = true;
                    newCoinReward.type = COIN;
                    newCoinReward.sprite.setTexture(mCoinTexture);
                    newCoinReward.sprite.setScale(COIN_SCALE, COIN_SCALE);
                    sf::FloatRect coinBounds = newCoinReward.sprite.getLocalBounds();
                    newCoinReward.sprite.setOrigin(coinBounds.width / 2.f, coinBounds.height / 2.f);
                    newCoinReward.sprite.setPosition(rewardPos.x + (rand() % 10 - 5), rewardPos.y + (rand() % 10 - 5));
                    mActiveRewards.push_back(newCoinReward); // Thêm vào danh sách item đang bay
                }
            }
            else if (boxIter->reward == MUSHROOM) {
                // (Tạo Reward nấm và thêm vào mActiveRewards)
                Reward newReward; newReward.homing = true; newReward.type = MUSHROOM; newReward.sprite.setTexture(mMushroomTexture); newReward.sprite.setScale(MUSHROOM_SCALE, MUSHROOM_SCALE);
                sf::FloatRect rewardBounds = newReward.sprite.getLocalBounds(); newReward.sprite.setOrigin(rewardBounds.width / 2.f, rewardBounds.height / 2.f); newReward.sprite.setPosition(rewardPos);
                mActiveRewards.push_back(newReward);
            }
            else { // FIRE_FLOWER
                // (Tạo Reward hoa và thêm vào mActiveRewards)
                Reward newReward; newReward.homing = true; newReward.type = FIRE_FLOWER; newReward.sprite.setTexture(mFireFlowerTexture); newReward.sprite.setScale(FIRE_FLOWER_SCALE, FIRE_FLOWER_SCALE);
                sf::FloatRect rewardBounds = newReward.sprite.getLocalBounds(); newReward.sprite.setOrigin(rewardBounds.width / 2.f, rewardBounds.height / 2.f); newReward.sprite.setPosition(rewardPos);
                mActiveRewards.push_back(newReward);
            }
            boxIter = mMysteryBoxes.erase(boxIter); // Xóa hộp
        }
        else {
            ++boxIter;
        }
    }

    // --- COLLISION (Player vs Active Rewards) --- (Từ updatePlaying())
    // (Va chạm với các item đang bay)
    for (auto rewardIter = mActiveRewards.begin(); rewardIter != mActiveRewards.end(); ) {
        if (playerBounds.intersects(rewardIter->sprite.getGlobalBounds())) {
            // Kiểm tra loại quà và tác động đến Player
            if (rewardIter->type == COIN) {
                player.addCoin(1);
                player.addScore(10);
                mCoinSound.play(); // Phát âm thanh xu (đã giảm)
            }
            else if (rewardIter->type == MUSHROOM) {
                player.addLife();
                player.addScore(10);
                player.collectPowerup(); // Hiệu ứng nháy xanh
                mPowerupSound.play(); // Phát âm thanh nấm (đã giảm)
            }
            else { // FIRE_FLOWER
                player.takeDamage(); // Mất máu
                mDamageSound.play(); // Phát âm thanh damage (giữ nguyên)
                if (player.isDead()) break; // Nếu chết thì dừng kiểm tra
            }
            rewardIter = mActiveRewards.erase(rewardIter); // Xóa quà
        }
        else {
            ++rewardIter;
        }
    }

    if (player.isDead()) return; // Thoát sớm nếu player đã chết

    // --- COLLISION (Player vs Ground Fire Flowers) --- (Từ updatePlaying())
    for (auto flowerIter = mGroundFireFlowers.begin(); flowerIter != mGroundFireFlowers.end(); ) {
        if (playerBounds.intersects(flowerIter->getGlobalBounds())) {
            player.takeDamage(); // Mất máu
            mDamageSound.play(); // Phát âm thanh damage (giữ nguyên)
            if (player.isDead()) break; // Dừng nếu chết
            flowerIter = mGroundFireFlowers.erase(flowerIter); // Xóa hoa
        }
        else {
            ++flowerIter;
        }
    }
}

// --- Các hàm sinh item (lấy từ resetGame() cũ) ---
// (Đây là logic sinh ngẫu nhiên, được giữ nguyên)

void Item::spawnCoins(const std::vector<sf::RectangleShape>& platforms)
{
    int maxRetries = 100; // Số lần thử tối đa để tìm vị trí
    const float MIN_COIN_DISTANCE = 100.f; // Khoảng cách tối thiểu giữa các xu
    auto getSafeRandomX = [&]() -> float { // Hàm lambda (hàm nhỏ) để lấy X ngẫu nhiên
        return static_cast<float>(rand() % static_cast<int>(GAME_WIDTH - 500) + 400);
        };

    const int numberOfCoins = 25;
    int coinsPlaced = 0;
    for (int i = 0; i < numberOfCoins && mCoinTexture.getSize().x > 0; ++i) {
        int retryCount = 0; bool placed = false;
        while (!placed && retryCount < maxRetries) {
            retryCount++;
            sf::Sprite newCoin(mCoinTexture);
            newCoin.setScale(COIN_SCALE, COIN_SCALE);
            float coinX = getSafeRandomX();
            float coinY = static_cast<float>(rand() % static_cast<int>(WINDOW_HEIGHT - 300) + 100);
            newCoin.setPosition(coinX, coinY);
            bool overlaps = false; // Kiểm tra xem có bị trùng không
            for (const auto& p : platforms) { if (newCoin.getGlobalBounds().intersects(p.getGlobalBounds())) { overlaps = true; break; } } // Trùng platform
            if (overlaps) continue; // Thử lại
            for (const auto& c : mCoins) { // Trùng xu khác
                sf::Vector2f diff = newCoin.getPosition() - c.getPosition();
                float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
                if (distance < MIN_COIN_DISTANCE) { overlaps = true; break; }
            }
            if (!overlaps) { mCoins.push_back(newCoin); placed = true; coinsPlaced++; } // Vị trí OK, thêm vào
        }
    }
}

void Item::spawnBoxes(const std::vector<sf::RectangleShape>& platforms)
{
    int maxRetries = 100;
    const float MIN_BOX_DISTANCE = 400.f;
    auto getSafeRandomX = [&]() -> float {
        return static_cast<float>(rand() % static_cast<int>(GAME_WIDTH - 500) + 400);
        };

    const int numberOfBoxes = 7;
    int boxesPlaced = 0;
    for (int i = 0; i < numberOfBoxes && mMysteryBoxTexture.getSize().x > 0; ++i) {
        int retryCount = 0; bool placed = false;
        while (!placed && retryCount < maxRetries) {
            retryCount++;
            MysteryBox newBox; // Tạo hộp mới
            newBox.sprite.setTexture(mMysteryBoxTexture);
            newBox.sprite.setScale(BOX_SCALE, BOX_SCALE);
            float boxX = getSafeRandomX();
            float boxY = static_cast<float>(rand() % 201 + 200);
            newBox.sprite.setPosition(boxX, boxY);

            // Quyết định quà bên trong (tỷ lệ 60% xu, 25% nấm, 15% hoa)
            int rewardRand = rand() % 100;
            if (rewardRand < 60) newBox.reward = COIN;
            else if (rewardRand < 85) newBox.reward = MUSHROOM;
            else newBox.reward = FIRE_FLOWER;

            bool overlaps = false;
            for (const auto& p : platforms) { if (newBox.sprite.getGlobalBounds().intersects(p.getGlobalBounds())) { overlaps = true; break; } }
            if (overlaps) continue;

            for (const auto& box : mMysteryBoxes) { // Kiểm tra trùng hộp khác
                sf::Vector2f diff = newBox.sprite.getPosition() - box.sprite.getPosition();
                float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
                if (distance < MIN_BOX_DISTANCE) {
                    overlaps = true;
                    break;
                }
            }
            if (!overlaps) { mMysteryBoxes.push_back(newBox); placed = true; boxesPlaced++; }
        }
    }
}

void Item::spawnFireFlowers(const std::vector<sf::RectangleShape>& platforms, const sf::RectangleShape& ground)
{
    int maxRetries = 100;
    const float MIN_FIRE_FLOWER_DISTANCE = 200.f;
    auto getSafeRandomX = [&]() -> float {
        return static_cast<float>(rand() % static_cast<int>(GAME_WIDTH - 500) + 400);
        };

    // Sinh 7 hoa lửa trên platform cao
    const int numberOfPlatformFlowers = 7;
    int platformFlowersPlaced = 0;
    for (int i = 0; i < numberOfPlatformFlowers && mFireFlowerTexture.getSize().x > 0 && platforms.size() > 1; ++i) {
        int retryCount = 0; bool placed = false;
        while (!placed && retryCount < maxRetries) {
            retryCount++;
            int platformIndex = rand() % (platforms.size() - 1) + 1; // Chọn platform ngẫu nhiên (trừ mặt đất)
            const auto& chosenPlatform = platforms[platformIndex];
            if (chosenPlatform.getPosition().x < 200) { continue; } // Tránh sinh quá gần điểm bắt đầu
            sf::Sprite newFlower(mFireFlowerTexture);
            newFlower.setScale(FIRE_FLOWER_SCALE, FIRE_FLOWER_SCALE);
            sf::FloatRect flowerBounds = newFlower.getLocalBounds();
            // Đặt hoa ở giữa platform
            float flowerX = chosenPlatform.getPosition().x + (chosenPlatform.getSize().x / 2.f) - (flowerBounds.width * FIRE_FLOWER_SCALE / 2.f);
            float flowerY = chosenPlatform.getPosition().y - (flowerBounds.height * FIRE_FLOWER_SCALE) + 5.f;
            newFlower.setPosition(flowerX, flowerY);
            bool overlaps = false;
            for (const auto& f : mGroundFireFlowers) { // Kiểm tra trùng hoa khác
                sf::Vector2f diff = newFlower.getPosition() - f.getPosition();
                float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
                if (newFlower.getGlobalBounds().intersects(f.getGlobalBounds()) || distance < MIN_FIRE_FLOWER_DISTANCE) {
                    overlaps = true; break;
                }
            }
            if (!overlaps) { mGroundFireFlowers.push_back(newFlower); placed = true; platformFlowersPlaced++; }
        }
    }

    // Sinh 8 hoa lửa dưới đất
    const int numberOfGroundFlowers = 8;
    int groundFlowersPlaced = 0;
    float groundTopY = ground.getPosition().y;
    for (int i = 0; i < numberOfGroundFlowers && mFireFlowerTexture.getSize().x > 0; ++i) {
        int retryCount = 0; bool placed = false;
        while (!placed && retryCount < maxRetries) {
            retryCount++;
            sf::Sprite newFlower(mFireFlowerTexture);
            newFlower.setScale(FIRE_FLOWER_SCALE, FIRE_FLOWER_SCALE);
            sf::FloatRect flowerBounds = newFlower.getLocalBounds();
            float flowerX = getSafeRandomX(); // Vị trí X ngẫu nhiên
            float flowerY = groundTopY - (flowerBounds.height * FIRE_FLOWER_SCALE) + 5.f; // Đặt trên mặt đất
            newFlower.setPosition(flowerX, flowerY);
            bool overlaps = false;
            for (const auto& f : mGroundFireFlowers) { // Kiểm tra trùng hoa khác
                sf::Vector2f diff = newFlower.getPosition() - f.getPosition();
                float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
                if (newFlower.getGlobalBounds().intersects(f.getGlobalBounds()) || distance < MIN_FIRE_FLOWER_DISTANCE) {
                    overlaps = true; break;
                }
            }
            if (!overlaps) { mGroundFireFlowers.push_back(newFlower); placed = true; groundFlowersPlaced++; }
        }
    }
}