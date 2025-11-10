// *** DÒNG NÀY CỰC KỲ QUAN TRỌNG! ***
// (Nó liên kết file .cpp này với file .h, và giúp nó "nhìn thấy" Defines.h)
#include "Player.h"

// Hàm khởi tạo: chỉ tạo hitbox (vì assets chỉ nên load 1 lần)
// :: là "toán tử phạm vi", nghĩa là hàm Player() này thuộc về class Player
Player::Player()
{
    // --- VẬT LÝ & HITBOX --- (Lấy từ main() cũ)
    sf::Vector2f playerHitboxSize(40.f, 65.f);
    mHitbox.setSize(playerHitboxSize);
    mHitbox.setFillColor(sf::Color::Transparent); // Hitbox sẽ trong suốt khi chạy game
}

// Tải assets của player (được gọi bởi class Game)
void Player::loadAssets()
{
    // --- Tải Texture (Hình ảnh) ---
    if (!mTexture.loadFromFile("Assets/mario_sheet.png")) {
        std::cerr << "Loi tai Assets/mario_sheet.png" << std::endl;
    }
    mSprite.setTexture(mTexture); // Gắn tấm ảnh vào sprite
    mSprite.setScale(0.25f, 0.25f); // Thu nhỏ hình ảnh

    // --- ANIMATION --- (Lấy từ main() cũ)
    // Cắt frame đầu tiên (đứng yên)
    mSprite.setTextureRect(sf::IntRect(IDLE_FRAME_INDEX * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT));

    // --- TẢI ÂM THANH (Đã giảm âm lượng nhảy) ---
    if (!mJumpBuffer.loadFromFile("Assets/jump.wav"))
    {
        std::cerr << "Loi tai Assets/jump.wav" << std::endl;
    }
    mJumpSound.setBuffer(mJumpBuffer); // Gắn file âm thanh vào "loa"
    mJumpSound.setVolume(50.f); // <-- ĐÃ GIẢM ÂM LƯỢNG NHẢY (50%)
}

// Reset player về trạng thái ban đầu
void Player::reset()
{
    // --- LOGIC TỪ resetGame() CŨ ---
    // (Thiết lập lại tất cả các biến về giá trị mặc định khi chơi lại)
    mScore = 0;
    mCoinsCollected = 0;
    mLives = 3;
    mSprite.setColor(sf::Color::White); // Xóa hiệu ứng màu (nếu có)
    mPlayerHit = false;
    mPlayerHitTimer = 0.f;
    mPlayerPowerup = false;
    mPlayerPowerupTimer = 0.f;
    mFacingRight = true;
    mCurrentAnimationFrame = 0;
    mAnimTimer = 0.f;
    mIsDead = false;

    // Đặt lại vị trí và vận tốc
    mHitbox.setPosition(100.0f, WINDOW_HEIGHT - 170.f); // Vị trí xuất phát
    mVelocity = sf::Vector2f(0.f, 0.f); // Đứng yên
}

// Xử lý event (chỉ dùng cho các hành động 1 lần như nhảy)
void Player::handleEvent(sf::Event event)
{
    // --- LOGIC TỪ VÒNG LẶP EVENT (main()) ---
    if (event.type == sf::Event::KeyPressed) // Nếu có phím được nhấn
    {
        // Nếu nhấn phím nhảy (W, Up, Space) VÀ đang trên mặt đất
        if ((event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Space) && mOnGround)
        {
            mVelocity.y = JUMP_POWER; // Gán vận tốc âm (để bay lên)
            mOnGround = false; // Rời khỏi mặt đất
            mJumpSound.play(); // <-- Phát âm thanh nhảy
        }
    }
}

// Cập nhật logic chính của Player mỗi frame
void Player::update(float deltaTime, const std::vector<sf::RectangleShape>& platforms)
{
    // --- INPUT (Trái/Phải - giữ phím) --- (Từ updatePlaying())
    // (Input di chuyển được check mỗi frame, không phải dạng "event")
    bool isMoving = false; // Giả sử đang đứng yên
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        mVelocity.x = -MOVE_SPEED;
        mFacingRight = false;
        isMoving = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        mVelocity.x = MOVE_SPEED;
        mFacingRight = true;
        isMoving = true;
    }
    else {
        mVelocity.x = 0.0f; // Đứng yên
        isMoving = false;
    }

    // --- CẬP NHẬT LOGIC (theo thứ tự) ---
    updateEffects(deltaTime); // 1. Cập nhật nháy
    updateAnimation(deltaTime, isMoving); // 2. Cập nhật hình ảnh
    applyPhysics(deltaTime); // 3. Thêm trọng lực vào mVelocity.y

    // 4. Tính toán độ di chuyển dự kiến
    sf::Vector2f displacement(mVelocity.x * deltaTime * 60.f, mVelocity.y * deltaTime * 60.f);

    // 5. Kiểm tra va chạm với platform (quan trọng nhất)
    checkCollisions(platforms, displacement);

    // --- GIỚI HẠN PLAYER --- (Không cho ra khỏi biên giới game)
    sf::FloatRect playerBounds = mHitbox.getGlobalBounds();
    if (mHitbox.getPosition().x < 0.f) { // Nếu đi quá bên trái
        mHitbox.setPosition(0.f, mHitbox.getPosition().y);
        mVelocity.x = 0;
    }
    if (mHitbox.getPosition().x + playerBounds.width > GAME_WIDTH) { // Nếu đi quá bên phải
        mHitbox.setPosition(GAME_WIDTH - playerBounds.width, mHitbox.getPosition().y);
        mVelocity.x = 0;
    }
}

// Vẽ Player
void Player::render(sf::RenderTarget& target)
{
    // --- CẬP NHẬT VỊ TRÍ SPRITE ---
    // (Hình ảnh (mSprite) cần được đặt lại vị trí sao cho khớp với hộp va chạm (mHitbox))
    // (Vì hitbox nhỏ hơn hình ảnh, ta cần căn chỉnh nó)
    sf::Vector2f visualSize = sf::Vector2f(FRAME_WIDTH * mSprite.getScale().x, FRAME_HEIGHT * mSprite.getScale().y);
    float offsetX = (visualSize.x - mHitbox.getSize().x) / 2.f; // Căn giữa theo X
    float offsetY = (visualSize.y - mHitbox.getSize().y); // Căn đáy theo Y
    mSprite.setPosition(mHitbox.getPosition().x - offsetX,
        mHitbox.getPosition().y - offsetY + ADJUSTMENT_Y); // (ADJUSTMENT_Y để chỉnh cho đẹp mắt)

    // --- VẼ (RENDER) ---
    target.draw(mSprite); // Vẽ hình ảnh player
}

// === CÁC HÀM PRIVATE (HELPER) ===

// Cập nhật hiệu ứng nhấp nháy
void Player::updateEffects(float deltaTime)
{
    // --- CẬP NHẬT HIỆU ỨNG NHÁY --- (Từ updatePlaying())
    if (mPlayerHit) { // Nếu đang nháy đỏ
        mPlayerHitTimer -= deltaTime; // Giảm thời gian
        if (mPlayerHitTimer <= 0.f) {
            mPlayerHit = false; // Hết hiệu ứng
            mSprite.setColor(sf::Color::White); // Trả về màu bình thường
        }
        else {
            // Logic chớp tắt: (int)(số_thực * 10) % 2 == 0 là một mẹo để tạo hiệu ứng chớp 5 lần/giây
            bool showRed = static_cast<int>(mPlayerHitTimer * 10.f) % 2 == 0;
            mSprite.setColor(showRed ? sf::Color::Red : sf::Color::White);
        }
    }
    else if (mPlayerPowerup) { // Tương tự, nếu đang nháy xanh
        mPlayerPowerupTimer -= deltaTime;
        if (mPlayerPowerupTimer <= 0.f) {
            mPlayerPowerup = false;
            mSprite.setColor(sf::Color::White);
        }
        else {
            bool showGreen = static_cast<int>(mPlayerPowerupTimer * 20.f) % 2 == 0; // Chớp nhanh hơn
            mSprite.setColor(showGreen ? sf::Color::Green : sf::Color::White);
        }
    }
    else {
        mSprite.setColor(sf::Color::White); // Không có hiệu ứng
    }
}

// Cập nhật frame animation
void Player::updateAnimation(float deltaTime, bool isMoving)
{
    // --- ANIMATION --- (Từ updatePlaying())
    if (isMoving) { // Nếu đang chạy
        mAnimTimer += deltaTime;
        if (mAnimTimer >= ANIM_SPEED) { // Đã đến lúc đổi frame
            mAnimTimer -= ANIM_SPEED; // Reset timer
            mCurrentAnimationFrame++; // Chuyển sang frame tiếp
            if (mCurrentAnimationFrame > LAST_RUN_FRAME_INDEX) {
                mCurrentAnimationFrame = FIRST_RUN_FRAME_INDEX; // Quay lại frame chạy đầu tiên
            }
        }
    }
    else { // Nếu đứng yên
        mCurrentAnimationFrame = IDLE_FRAME_INDEX; // Đặt về frame đứng yên
        mAnimTimer = 0.f;
    }

    // Cắt hình chữ nhật (IntRect) từ tấm ảnh sprite sheet
    sf::IntRect animRect;
    animRect.top = 0;
    animRect.height = FRAME_HEIGHT;
    animRect.width = FRAME_WIDTH;
    animRect.left = mCurrentAnimationFrame * FRAME_WIDTH; // Vị trí cắt ngang

    // Lật hình nếu quay sang trái
    if (!mFacingRight) {
        animRect.left += animRect.width; // Di chuyển điểm bắt đầu cắt sang phải
        animRect.width = -animRect.width; // Đặt chiều rộng âm (mẹo của SFML để lật hình)
    }
    mSprite.setTextureRect(animRect); // Áp dụng hình chữ nhật đã cắt cho sprite
}

// Áp dụng trọng lực
void Player::applyPhysics(float deltaTime)
{
    mVelocity.y += GRAVITY; // Luôn tăng vận tốc rơi (kéo xuống)
}

// Kiểm tra va chạm với các platform (Xử lý tách trục)
void Player::checkCollisions(const std::vector<sf::RectangleShape>& platforms, sf::Vector2f displacement)
{
    // --- PHYSICS UPDATE (Tách trục) --- (Từ updatePlaying())
    // (Đây là kỹ thuật va chạm AABB - Tách X và Y để xử lý trượt tường)
    mOnGround = false; // Giả sử không chạm đất, sẽ đặt lại = true nếu va chạm bên dưới

    // 1. Di chuyển X và Kiểm tra X
    mHitbox.move(displacement.x, 0.f);
    sf::FloatRect playerBounds = mHitbox.getGlobalBounds();
    for (const auto& platform : platforms) {
        sf::FloatRect platformBounds = platform.getGlobalBounds();
        if (playerBounds.intersects(platformBounds)) { // Nếu va chạm ngang
            if (displacement.x > 0) { mHitbox.setPosition(platformBounds.left - playerBounds.width, playerBounds.top); } // Đụng bên phải
            else if (displacement.x < 0) { mHitbox.setPosition(platformBounds.left + platformBounds.width, playerBounds.top); } // Đụng bên trái
            mVelocity.x = 0; // Dừng di chuyển ngang
            playerBounds = mHitbox.getGlobalBounds(); // Cập nhật lại bounds sau khi di chuyển
        }
    }
    // 2. Di chuyển Y và Kiểm tra Y
    mHitbox.move(0.f, displacement.y);
    playerBounds = mHitbox.getGlobalBounds();
    for (const auto& platform : platforms) {
        sf::FloatRect platformBounds = platform.getGlobalBounds();
        if (playerBounds.intersects(platformBounds)) { // Nếu va chạm dọc
            if (displacement.y > 0) { mHitbox.setPosition(playerBounds.left, platformBounds.top - playerBounds.height); mVelocity.y = 0.f; mOnGround = true; } // Đứng trên
            else if (displacement.y < 0) { mHitbox.setPosition(playerBounds.left, platformBounds.top + platformBounds.height); mVelocity.y = 0.f; } // Đụng đầu
            playerBounds = mHitbox.getGlobalBounds(); // Cập nhật lại bounds
        }
    }
}


// === CÁC HÀM GETTER/SETTER ===
// (Các hàm public, giúp các class khác tương tác với Player)
sf::FloatRect Player::getGlobalBounds() const { return mHitbox.getGlobalBounds(); }
sf::Vector2f Player::getPosition() const { return mHitbox.getPosition(); }
sf::Vector2f Player::getCenter() const { return mHitbox.getPosition() + (mHitbox.getSize() / 2.f); }
bool Player::isHit() const { return mPlayerHit; }
int Player::getScore() const { return mScore; }
int Player::getCoins() const { return mCoinsCollected; }
int Player::getLives() const { return mLives; }
bool Player::isDead() const { return mIsDead; }

void Player::addScore(int points) { mScore += points; }
void Player::addCoin(int amount) { mCoinsCollected += amount; }
void Player::addLife() { mLives++; }
void Player::collectPowerup() {
    if (!mPlayerHit) { // Chỉ ăn nấm khi không đang bị thương
        mPlayerPowerup = true;
        mPlayerPowerupTimer = PLAYER_POWERUP_DURATION;
    }
}
void Player::setOnGround(bool onGround) { mOnGround = onGround; }
void Player::setGameOver() { mIsDead = true; }

// Xử lý khi nhận sát thương
void Player::takeDamage() {
    if (!mPlayerHit) { // Chỉ mất máu khi không đang trong thời gian bất tử (nháy đỏ)
        mLives--;
        mPlayerHit = true; // Bật trạng thái bất tử tạm thời
        mPlayerHitTimer = PLAYER_HIT_DURATION;
        mScore -= 5;
        if (mLives <= 0) {
            mIsDead = true; // Báo cho class Game biết là đã thua
            // (Đã xóa âm thanh death ở đây)
        }
    }
}