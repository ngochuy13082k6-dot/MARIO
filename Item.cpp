#include "Item.h"
#include "Player.h" 
#include <iostream> 
#include <cmath> // Cần cho std::sqrt

Item::Item() {}

sf::Vector2f Item::normalize(const sf::Vector2f& source) {
    float length = std::sqrt((source.x * source.x) + (source.y * source.y));
    if (length != 0) { return sf::Vector2f(source.x / length, source.y / length); }
    else { return source; }
}

void Item::loadAssets()
{
    if (!mCoinTexture.loadFromFile("Assets/coin.png")) { /* Xử lý lỗi */ }
    if (!mMysteryBoxTexture.loadFromFile("Assets/mystery_box.png")) { /* Xử lý lỗi */ }
    if (!mMushroomTexture.loadFromFile("Assets/mushroom.png")) { /* Xử lý lỗi */ }
    if (!mFireFlowerTexture.loadFromFile("Assets/fire_flower.png")) { /* Xử lý lỗi */ }
    if (!mCoinBuffer.loadFromFile("Assets/coin.wav")) { /* Xử lý lỗi */ }
    mCoinSound.setBuffer(mCoinBuffer); mCoinSound.setVolume(50.f);
    if (!mPowerupBuffer.loadFromFile("Assets/powerup.wav")) { /* Xử lý lỗi */ }
    mPowerupSound.setBuffer(mPowerupBuffer); mPowerupSound.setVolume(50.f);
    if (!mDamageBuffer.loadFromFile("Assets/damage.wav")) { /* Xử lý lỗi */ }
    mDamageSound.setBuffer(mDamageBuffer);
}

// Hàm Reset: Đọc config và gọi các hàm spawn
void Item::reset(int level, const std::vector<sf::RectangleShape>& platforms, sf::FloatRect flagBounds)
{
    mCoins.clear(); mMysteryBoxes.clear(); mActiveRewards.clear(); mGroundFireFlowers.clear();

    if (platforms.empty()) return;

    std::string configFile = "Assets/level" + std::to_string(level) + "_config.txt";
    std::ifstream file(configFile);

    int numCoins = 25, numBoxes = 7, numFlowers = 15; // Giá trị mặc định

    if (file.is_open()) {
        std::string type; int count;
        while (file >> type >> count) {
            if (type == "COINS") { numCoins = count; }
            else if (type == "BOXES") { numBoxes = count; }
            else if (type == "FLOWERS") { numFlowers = count; }
        }
        file.close();
    }
    else {
        std::cerr << "Warning: Config file not found: " << configFile << ". Using defaults." << std::endl;
    }

    // Gọi các hàm spawn với vị trí cờ (flagBounds) được truyền vào
    spawnCoins(numCoins, platforms, flagBounds);
    spawnBoxes(numBoxes, platforms, flagBounds);
    spawnFireFlowers(numFlowers, platforms, platforms[0], flagBounds);
}

void Item::update(float deltaTime, Player& player)
{
    updateActiveRewards(deltaTime, player);
    checkCollisions(player);
}

void Item::render(sf::RenderTarget& target)
{
    for (const auto& box : mMysteryBoxes) { target.draw(box.sprite); }
    for (const auto& coin : mCoins) { target.draw(coin); }
    for (const auto& reward : mActiveRewards) { target.draw(reward.sprite); }
    for (const auto& flower : mGroundFireFlowers) { target.draw(flower); }
}

void Item::updateActiveRewards(float deltaTime, Player& player)
{
    // Logic Homing (Item bay về phía Mario)
    sf::Vector2f playerCenter = player.getCenter();
    for (auto& reward : mActiveRewards) {
        if (reward.homing) {
            sf::Vector2f direction = playerCenter - reward.sprite.getPosition();
            reward.velocity = normalize(direction) * HOMING_SPEED;
            reward.sprite.move(reward.velocity * deltaTime * 60.f);
        }
    }
}

// --- HÀM XỬ LÝ VA CHẠM VÀ TÍNH ĐIỂM ---
void Item::checkCollisions(Player& player)
{
    sf::FloatRect playerBounds = player.getGlobalBounds();

    // 1. VA CHẠM COINS
    for (auto coinIter = mCoins.begin(); coinIter != mCoins.end(); ) {
        if (playerBounds.intersects(coinIter->getGlobalBounds())) {
            coinIter = mCoins.erase(coinIter);
            player.addCoin(1); player.addScore(10); mCoinSound.play();
        }
        else { ++coinIter; }
    }

    // 2. VA CHẠM BOXES (Đụng hộp: +10 điểm)
    for (auto boxIter = mMysteryBoxes.begin(); boxIter != mMysteryBoxes.end(); ) {
        if (playerBounds.intersects(boxIter->sprite.getGlobalBounds())) {
            player.addScore(10); // ĐỤNG HỘP: CỘNG 10 ĐIỂM

            sf::Vector2f rewardPos = { boxIter->sprite.getPosition().x, boxIter->sprite.getPosition().y };
            // Logic tạo reward (Coin, Nấm, Hoa)
            if (boxIter->reward == COIN) {
                int numCoinsToSpawn = 1; int coinAmountRand = rand() % 100;
                if (coinAmountRand < 10) numCoinsToSpawn = 3; else if (coinAmountRand < 30) numCoinsToSpawn = 2;
                else numCoinsToSpawn = 1;
                for (int c = 0; c < numCoinsToSpawn; ++c) {
                    Reward newCoinReward; newCoinReward.homing = true; newCoinReward.type = COIN; newCoinReward.sprite.setTexture(mCoinTexture); newCoinReward.sprite.setScale(COIN_SCALE, COIN_SCALE);
                    sf::FloatRect coinBounds = newCoinReward.sprite.getLocalBounds(); newCoinReward.sprite.setOrigin(coinBounds.width / 2.f, coinBounds.height / 2.f);
                    newCoinReward.sprite.setPosition(rewardPos.x + (rand() % 10 - 5), rewardPos.y + (rand() % 10 - 5));
                    mActiveRewards.push_back(newCoinReward);
                }
            }
            else if (boxIter->reward == MUSHROOM) {
                Reward newReward; newReward.homing = true; newReward.type = MUSHROOM; newReward.sprite.setTexture(mMushroomTexture); newReward.sprite.setScale(MUSHROOM_SCALE, MUSHROOM_SCALE);
                sf::FloatRect rewardBounds = newReward.sprite.getLocalBounds(); newReward.sprite.setOrigin(rewardBounds.width / 2.f, rewardBounds.height / 2.f); newReward.sprite.setPosition(rewardPos);
                mActiveRewards.push_back(newReward);
            }
            else {
                Reward newReward; newReward.homing = true; newReward.type = FIRE_FLOWER; newReward.sprite.setTexture(mFireFlowerTexture); newReward.sprite.setScale(FIRE_FLOWER_SCALE, FIRE_FLOWER_SCALE);
                sf::FloatRect rewardBounds = newReward.sprite.getLocalBounds(); newReward.sprite.setOrigin(rewardBounds.width / 2.f, rewardBounds.height / 2.f); newReward.sprite.setPosition(rewardPos);
                mActiveRewards.push_back(newReward);
            }
            boxIter = mMysteryBoxes.erase(boxIter);
        }
        else { ++boxIter; }
    }

    // 3. VA CHẠM ACTIVE REWARDS (Nấm: +30, Hoa Lửa bay ra từ hộp: -20)
    for (auto rewardIter = mActiveRewards.begin(); rewardIter != mActiveRewards.end(); ) {
        if (playerBounds.intersects(rewardIter->sprite.getGlobalBounds())) {
            if (rewardIter->type == COIN) { player.addCoin(1); player.addScore(10); mCoinSound.play(); }
            else if (rewardIter->type == MUSHROOM) {
                player.addLife();
                player.addScore(30); // ĂN NẤM: CỘNG 30 ĐIỂM
                player.collectPowerup();
                mPowerupSound.play();
            }
            else { // HOA LỬA BAY RA TỪ HỘP
                player.addScore(-20); // TRỪ 20 ĐIỂM (ĐÃ CẬP NHẬT)
                player.takeDamage(); // Mất 1 mạng + I-frames
                mDamageSound.play();
                if (player.isDead()) break;
            }
            rewardIter = mActiveRewards.erase(rewardIter);
        }
        else { ++rewardIter; }
    }

    // 4. VA CHẠM GROUND FLOWERS (Hoa Lửa dưới đất: -20)
    if (player.isDead()) return;
    for (auto flowerIter = mGroundFireFlowers.begin(); flowerIter != mGroundFireFlowers.end(); ) {
        if (playerBounds.intersects(flowerIter->getGlobalBounds())) {
            player.addScore(-20); // BỊ ĐÁNH THƯỜNG (HOA LỬA DƯỚI ĐẤT): TRỪ 20 ĐIỂM (ĐÃ CẬP NHẬT)
            player.takeDamage(); // Mất 1 mạng + I-frames
            mDamageSound.play();
            if (player.isDead()) break;
            flowerIter = mGroundFireFlowers.erase(flowerIter);
        }
        else { ++flowerIter; }
    }
}

// --- HÀM HELPER (RANDOM ITEM SPAWN) ---

void Item::spawnCoins(int count, const std::vector<sf::RectangleShape>& platforms, sf::FloatRect flagBounds)
{
    int maxRetries = 100;
    const float MIN_COIN_DISTANCE = 100.f;

    // Item bắt đầu spawn từ X=200 (Vùng an toàn)
    auto getSafeRandomX = [&]() -> float {
        return static_cast<float>(rand() % static_cast<int>(GAME_WIDTH - 400) + 200);
        };
    // Item rải đều từ Y=100 (trên cao) đến Y=650 (gần mặt đất)
    auto getSafeRandomY = [&]() -> float {
        return static_cast<float>(rand() % 551 + 100);
        };

    for (int i = 0; i < count && mCoinTexture.getSize().x > 0; ++i) {
        int retryCount = 0; bool placed = false;
        while (!placed && retryCount < maxRetries) {
            retryCount++;
            sf::Sprite newCoin(mCoinTexture); newCoin.setScale(COIN_SCALE, COIN_SCALE);
            float coinX = getSafeRandomX(); float coinY = getSafeRandomY(); newCoin.setPosition(coinX, coinY);

            bool overlaps = false;
            // 1. Kiểm tra va chạm với Platform
            for (const auto& p : platforms) { if (newCoin.getGlobalBounds().intersects(p.getGlobalBounds())) { overlaps = true; break; } }
            if (overlaps) continue;
            // 2. Kiểm tra va chạm với Flag
            if (newCoin.getGlobalBounds().intersects(flagBounds)) { overlaps = true; continue; }
            // 3. Kiểm tra va chạm với Coin khác
            for (const auto& c : mCoins) {
                sf::Vector2f diff = newCoin.getPosition() - c.getPosition(); float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
                if (distance < MIN_COIN_DISTANCE) { overlaps = true; break; }
            }
            if (!overlaps) { mCoins.push_back(newCoin); placed = true; }
        }
    }
}

void Item::spawnBoxes(int count, const std::vector<sf::RectangleShape>& platforms, sf::FloatRect flagBounds)
{
    int maxRetries = 100;
    const float MIN_BOX_DISTANCE = 400.f;

    // Item bắt đầu spawn từ X=200 (Vùng an toàn)
    auto getSafeRandomX = [&]() -> float {
        return static_cast<float>(rand() % static_cast<int>(GAME_WIDTH - 400) + 200);
        };
    auto getSafeRandomY = [&]() -> float {
        return static_cast<float>(rand() % 551 + 100);
        };

    for (int i = 0; i < count && mMysteryBoxTexture.getSize().x > 0; ++i) {
        int retryCount = 0; bool placed = false;
        while (!placed && retryCount < maxRetries) {
            retryCount++;
            MysteryBox newBox; newBox.sprite.setTexture(mMysteryBoxTexture); newBox.sprite.setScale(BOX_SCALE, BOX_SCALE);
            float boxX = getSafeRandomX(); float boxY = getSafeRandomY(); newBox.sprite.setPosition(boxX, boxY);
            int rewardRand = rand() % 100; // Logic random loại phần thưởng
            if (rewardRand < 60) newBox.reward = COIN; else if (rewardRand < 85) newBox.reward = MUSHROOM; else newBox.reward = FIRE_FLOWER;

            bool overlaps = false;
            // 1. Kiểm tra va chạm với Platform
            for (const auto& p : platforms) { if (newBox.sprite.getGlobalBounds().intersects(p.getGlobalBounds())) { overlaps = true; break; } }
            if (overlaps) continue;
            // 2. Kiểm tra va chạm với Flag
            if (newBox.sprite.getGlobalBounds().intersects(flagBounds)) { overlaps = true; continue; }
            // 3. Kiểm tra va chạm với Box khác
            for (const auto& box : mMysteryBoxes) {
                sf::Vector2f diff = newBox.sprite.getPosition() - box.sprite.getPosition();
                float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
                if (distance < MIN_BOX_DISTANCE) { overlaps = true; break; }
            }
            if (!overlaps) { mMysteryBoxes.push_back(newBox); placed = true; }
        }
    }
}

void Item::spawnFireFlowers(int count, const std::vector<sf::RectangleShape>& platforms, const sf::RectangleShape& ground, sf::FloatRect flagBounds)
{
    int maxRetries = 100;
    const float MIN_FIRE_FLOWER_DISTANCE = 200.f;
    const float SAFE_ZONE_X = 200.f; // Vùng an toàn 200px (tránh spawn kill)

    auto getSafeRandomX = [&]() -> float {
        // Bắt đầu từ 200. Phạm vi = GAME_WIDTH - 300
        return static_cast<float>(rand() % static_cast<int>(GAME_WIDTH - SAFE_ZONE_X - 100) + SAFE_ZONE_X);
        };

    const int numberOfPlatformFlowers = count / 2;
    const int numberOfGroundFlowers = count - numberOfPlatformFlowers;

    // Logic spawn trên Platform
    for (int i = 0; i < numberOfPlatformFlowers && mFireFlowerTexture.getSize().x > 0 && platforms.size() > 1; ++i) {
        int retryCount = 0; bool placed = false;
        while (!placed && retryCount < maxRetries) {
            retryCount++;
            int platformIndex = rand() % (platforms.size() - 1) + 1;
            const auto& chosenPlatform = platforms[platformIndex];

            // Kiểm tra vùng an toàn cho platform
            if (chosenPlatform.getPosition().x < SAFE_ZONE_X) { continue; }

            sf::Sprite newFlower(mFireFlowerTexture); newFlower.setScale(FIRE_FLOWER_SCALE, FIRE_FLOWER_SCALE);
            sf::FloatRect flowerBounds = newFlower.getLocalBounds();
            float flowerX = chosenPlatform.getPosition().x + (chosenPlatform.getSize().x / 2.f) - (flowerBounds.width * FIRE_FLOWER_SCALE / 2.f);
            float flowerY = chosenPlatform.getPosition().y - (flowerBounds.height * FIRE_FLOWER_SCALE) + 5.f;
            newFlower.setPosition(flowerX, flowerY);

            bool overlaps = false;
            // 1. Kiểm tra va chạm với Flag
            if (newFlower.getGlobalBounds().intersects(flagBounds)) { overlaps = true; continue; }
            // 2. Kiểm tra va chạm với hoa khác
            for (const auto& f : mGroundFireFlowers) {
                sf::Vector2f diff = newFlower.getPosition() - f.getPosition(); float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
                if (newFlower.getGlobalBounds().intersects(f.getGlobalBounds()) || distance < MIN_FIRE_FLOWER_DISTANCE) { overlaps = true; break; }
            }
            if (!overlaps) { mGroundFireFlowers.push_back(newFlower); placed = true; }
        }
    }

    // Logic spawn trên Ground (Mặt đất)
    float groundTopY = ground.getPosition().y;
    for (int i = 0; i < numberOfGroundFlowers && mFireFlowerTexture.getSize().x > 0; ++i) {
        int retryCount = 0; bool placed = false;
        while (!placed && retryCount < maxRetries) {
            retryCount++;
            sf::Sprite newFlower(mFireFlowerTexture); newFlower.setScale(FIRE_FLOWER_SCALE, FIRE_FLOWER_SCALE);
            sf::FloatRect flowerBounds = newFlower.getLocalBounds();

            float flowerX = getSafeRandomX();

            float flowerY = groundTopY - (flowerBounds.height * FIRE_FLOWER_SCALE) + 5.f;
            newFlower.setPosition(flowerX, flowerY);

            bool overlaps = false;
            // 1. Kiểm tra va chạm với Flag
            if (newFlower.getGlobalBounds().intersects(flagBounds)) { overlaps = true; continue; }
            // 2. Kiểm tra va chạm với hoa khác
            for (const auto& f : mGroundFireFlowers) {
                sf::Vector2f diff = newFlower.getPosition() - f.getPosition(); float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
                if (newFlower.getGlobalBounds().intersects(f.getGlobalBounds()) || distance < MIN_FIRE_FLOWER_DISTANCE) { overlaps = true; break; }
            }
            if (!overlaps) { mGroundFireFlowers.push_back(newFlower); placed = true; }
        }
    }
}