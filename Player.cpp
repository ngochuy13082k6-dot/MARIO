#include "Player.h" 

Player::Player()
{
    sf::Vector2f playerHitboxSize(40.f, 65.f);
    mHitbox.setSize(playerHitboxSize);
    mHitbox.setFillColor(sf::Color::Transparent);
}

void Player::loadAssets()
{
    if (!mTexture.loadFromFile("Assets/mario_sheet.png")) { /*...*/ }
    mSprite.setTexture(mTexture);
    mSprite.setScale(0.25f, 0.25f);
    mSprite.setTextureRect(sf::IntRect(IDLE_FRAME_INDEX * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT));
    if (!mJumpBuffer.loadFromFile("Assets/jump.wav")) { /*...*/ }
    mJumpSound.setBuffer(mJumpBuffer);
    mJumpSound.setVolume(50.f);
}

void Player::reset(bool resetAllStats)
{
    if (resetAllStats)
    {
        mScore = 0;
        mCoinsCollected = 0;
        mLives = 5;
    }
    mSprite.setColor(sf::Color::White);
    mPlayerHit = false;
    mPlayerHitTimer = 0.f;
    mPlayerPowerup = false;
    mPlayerPowerupTimer = 0.f;
    mFacingRight = true;
    mCurrentAnimationFrame = 0;
    mAnimTimer = 0.f;
    mIsDead = false;
    mHitbox.setPosition(100.0f, GAME_HEIGHT - 170.f);
    mVelocity = sf::Vector2f(0.f, 0.f);
}

void Player::handleEvent(sf::Event event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if ((event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Space) && mOnGround)
        {
            mVelocity.y = JUMP_POWER;
            mOnGround = false;
            mJumpSound.play();
        }
    }
}

void Player::update(float deltaTime, const std::vector<sf::RectangleShape>& platforms)
{
    bool isMoving = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) { mVelocity.x = -MOVE_SPEED; mFacingRight = false; isMoving = true; }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { mVelocity.x = MOVE_SPEED; mFacingRight = true; isMoving = true; }
    else { mVelocity.x = 0.0f; isMoving = false; }

    updateEffects(deltaTime); updateAnimation(deltaTime, isMoving); applyPhysics(deltaTime);
    sf::Vector2f displacement(mVelocity.x * deltaTime * 60.f, mVelocity.y * deltaTime * 60.f);
    checkCollisions(platforms, displacement);

    sf::FloatRect playerBounds = mHitbox.getGlobalBounds();
    if (mHitbox.getPosition().x < 0.f) { mHitbox.setPosition(0.f, mHitbox.getPosition().y); mVelocity.x = 0; }
    if (mHitbox.getPosition().x + playerBounds.width > GAME_WIDTH) { mHitbox.setPosition(GAME_WIDTH - playerBounds.width, mHitbox.getPosition().y); mVelocity.x = 0; }
}

void Player::render(sf::RenderTarget& target)
{
    sf::Vector2f visualSize = sf::Vector2f(FRAME_WIDTH * mSprite.getScale().x, FRAME_HEIGHT * mSprite.getScale().y);
    float offsetX = (visualSize.x - mHitbox.getSize().x) / 2.f;
    float offsetY = (visualSize.y - mHitbox.getSize().y);
    mSprite.setPosition(mHitbox.getPosition().x - offsetX, mHitbox.getPosition().y - offsetY + ADJUSTMENT_Y);
    target.draw(mSprite);
}

void Player::updateEffects(float deltaTime)
{
    if (mPlayerHit) {
        mPlayerHitTimer -= deltaTime;
        if (mPlayerHitTimer <= 0.f) { mPlayerHit = false; mSprite.setColor(sf::Color::White); }
        else { bool showRed = static_cast<int>(mPlayerHitTimer * 10.f) % 2 == 0; mSprite.setColor(showRed ? sf::Color::Red : sf::Color::White); }
    }
    else if (mPlayerPowerup) {
        mPlayerPowerupTimer -= deltaTime;
        if (mPlayerPowerupTimer <= 0.f) { mPlayerPowerup = false; mSprite.setColor(sf::Color::White); }
        else { bool showGreen = static_cast<int>(mPlayerPowerupTimer * 20.f) % 2 == 0; mSprite.setColor(showGreen ? sf::Color::Green : sf::Color::White); }
    }
    else { mSprite.setColor(sf::Color::White); }
}

void Player::updateAnimation(float deltaTime, bool isMoving)
{
    if (isMoving) {
        mAnimTimer += deltaTime;
        if (mAnimTimer >= ANIM_SPEED) {
            mAnimTimer -= ANIM_SPEED;
            mCurrentAnimationFrame++;
            if (mCurrentAnimationFrame > LAST_RUN_FRAME_INDEX) { mCurrentAnimationFrame = FIRST_RUN_FRAME_INDEX; }
        }
    }
    else { mCurrentAnimationFrame = IDLE_FRAME_INDEX; mAnimTimer = 0.f; }
    sf::IntRect animRect;
    animRect.top = 0; animRect.height = FRAME_HEIGHT; animRect.width = FRAME_WIDTH; animRect.left = mCurrentAnimationFrame * FRAME_WIDTH;
    if (!mFacingRight) { animRect.left += animRect.width; animRect.width = -animRect.width; } mSprite.setTextureRect(animRect);
}

void Player::applyPhysics(float deltaTime) { mVelocity.y += GRAVITY; }

void Player::checkCollisions(const std::vector<sf::RectangleShape>& platforms, sf::Vector2f displacement)
{
    mOnGround = false;
    mHitbox.move(displacement.x, 0.f); sf::FloatRect playerBounds = mHitbox.getGlobalBounds();
    for (const auto& platform : platforms) {
        sf::FloatRect platformBounds = platform.getGlobalBounds();
        if (playerBounds.intersects(platformBounds)) {
            if (displacement.x > 0) { mHitbox.setPosition(platformBounds.left - playerBounds.width, playerBounds.top); }
            else if (displacement.x < 0) { mHitbox.setPosition(platformBounds.left + platformBounds.width, playerBounds.top); }
            mVelocity.x = 0; playerBounds = mHitbox.getGlobalBounds();
        }
    }
    mHitbox.move(0.f, displacement.y); playerBounds = mHitbox.getGlobalBounds();
    for (const auto& platform : platforms) {
        sf::FloatRect platformBounds = platform.getGlobalBounds();
        if (playerBounds.intersects(platformBounds)) {
            if (displacement.y > 0) { mHitbox.setPosition(playerBounds.left, platformBounds.top - playerBounds.height); mVelocity.y = 0.f; mOnGround = true; }
            else if (displacement.y < 0) { mHitbox.setPosition(playerBounds.left, platformBounds.top + platformBounds.height); mVelocity.y = 0.f; }
            playerBounds = mHitbox.getGlobalBounds();
        }
    }
}

// --- ĐỊNH NGHĨA CÁC HÀM BỊ THIẾU ---
sf::FloatRect Player::getGlobalBounds() const { return mHitbox.getGlobalBounds(); }
sf::Vector2f Player::getPosition() const { return mHitbox.getPosition(); }
sf::Vector2f Player::getCenter() const { return mHitbox.getPosition() + (mHitbox.getSize() / 2.f); }
bool Player::isHit() const { return mPlayerHit; }
int Player::getScore() const { return mScore; }
int Player::getCoins() const { return mCoinsCollected; }
int Player::getLives() const { return mLives; }
bool Player::isDead() const { return mIsDead; }

// CÁC HÀM ADD/SET
void Player::addScore(int points) { mScore += points; }
void Player::addCoin(int amount) { mCoinsCollected += amount; }
void Player::addLife() { mLives++; }
void Player::setLives(int lives) { mLives = lives; }
void Player::setScore(int score) { mScore = score; }
void Player::setCoins(int coins) { mCoinsCollected = coins; }
void Player::collectPowerup() {
    if (!mPlayerHit) { mPlayerPowerup = true; mPlayerPowerupTimer = PLAYER_POWERUP_DURATION; }
}
void Player::setOnGround(bool onGround) { mOnGround = onGround; }
void Player::setGameOver() { mIsDead = true; }

// Hàm chỉ trừ MẠNG và kích hoạt I-Frames
void Player::takeDamage() {
    if (!mPlayerHit) {
        mLives--;
        mPlayerHit = true;
        mPlayerHitTimer = PLAYER_HIT_DURATION;
        if (mLives <= 0) { mIsDead = true; }
    }
}