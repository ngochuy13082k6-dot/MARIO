// *** DÒNG NÀY CỰC KỲ QUAN TRỌNG! ***
#include "Game.h"

// Hàm khởi tạo Game: tạo cửa sổ, tải tất cả tài nguyên
// (Dùng cú pháp "initializer list" (danh sách khởi tạo) để gán giá trị ban đầu)
Game::Game()
    : mWindow(sf::VideoMode(static_cast<unsigned int>(WINDOW_WIDTH), static_cast<unsigned int>(WINDOW_HEIGHT)), "Mario Game - OOP!")
    , mCurrentState(MENU) // Bắt đầu ở Menu
    , mStartingTimer(2.0f)
    , mGameOverTimer(5.0f)
    , mTitleTimer(5.0f)
{
    mWindow.setFramerateLimit(60); // Khóa game ở 60 FPS

    // --- TẢI ÂM THANH GAME ---
    if (!mBackgroundMusic.openFromFile("Assets/music.ogg"))
    {
        std::cerr << "Loi tai Assets/music.ogg" << std::endl;
    }
    mBackgroundMusic.setLoop(true); // Lặp lại
    mBackgroundMusic.setVolume(50); // 50% âm lượng
    mBackgroundMusic.play(); // <-- PHÁT NHẠC NỀN NGAY LẬP TỨC TỪ MENU

    if (!mGameOverBuffer.loadFromFile("Assets/gameover.wav"))
    {
        std::cerr << "Loi tai Assets/gameover.wav" << std::endl;
    }
    mGameOverSound.setBuffer(mGameOverBuffer);

    // --- KHỞI TẠO CAMERA ---
    mCamera.setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    mCloudView.setSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    // --- TẢI ASSETS CHO TỪNG ĐỐI TƯỢNG ---
    mPlayer.loadAssets();
    mItem.loadAssets();
    mMap.loadAssets();
    mUI.loadAssets();

    // --- CÀI ĐẶT TEXT (SAU KHI TẢI FONT) ---
    mUI.setupTexts(); // Chỉ gọi 1 lần
    loadHighScores(); // Tải điểm cao khi bắt đầu game
}

// Hàm chính, chứa vòng lặp game
void Game::run()
{
    sf::Clock clock; // Đồng hồ để đo deltaTime

    // --- GAME LOOP --- (Lấy từ main() cũ)
    while (mWindow.isOpen())
    {
        // Delta time là thời gian (giây) từ frame trước đến frame này
        float deltaTime = clock.restart().asSeconds();
        // Giới hạn delta time (tránh lỗi "dịch chuyển tức thời" khi game bị lag)
        if (deltaTime > 1.f / 20.f) {
            deltaTime = 1.f / 20.f; // (Tối đa là 20 FPS)
        }

        processEvents(); // 1. Xử lý Input
        update(deltaTime); // 2. Cập nhật Logic
        render(); // 3. Vẽ
    }
}

// Xử lý tất cả sự kiện (input không liên tục, vd: nhấn phím, đóng cửa sổ)
void Game::processEvents()
{
    // --- XỬ LÝ SỰ KIỆN (EVENT) --- (Lấy từ main() cũ)
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        if (event.type == sf::Event::Closed) {
            mWindow.close(); // Đóng cửa sổ
        }

        // Xử lý Input tùy theo trạng thái game (State Machine)
        switch (mCurrentState)
        {
        case MENU:
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                mCurrentState = TITLE_SCREEN; // Chuyển state
                mTitleTimer = 5.0f; // Đặt giờ

                // Nếu nhạc bị tắt (do vừa Game Over), bật lại
                if (mBackgroundMusic.getStatus() != sf::Music::Playing)
                {
                    mBackgroundMusic.play();
                }
            }
            break;
        case TITLE_SCREEN:
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                mCurrentState = STARTING; // Chuyển state
                mStartingTimer = 2.0f;
            }
            break;
        case PLAYING:
            // Gửi event cho Player (Player sẽ tự xử lý xem có phải nút nhảy không)
            mPlayer.handleEvent(event);

            // Event đầu hàng (Enter)
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                mCurrentState = GAME_OVER;
                mGameOverTimer = 5.0f;
                mBackgroundMusic.stop(); // Dừng nhạc nền
                mGameOverSound.play();   // Phát âm thanh Game Over
            }
            break;
        case SCORE_SCREEN:
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                mCurrentState = MENU; // Quay về Menu
                // (Không bật lại nhạc ở đây, để nhạc bật khi nhấn Enter ở Menu)
            }
            break;
        case GAME_OVER: // Không xử lý input
        case STARTING:  // Không xử lý input
            break;
        }
    }
}

// Cập nhật logic game (quản lý state, gọi update của các đối tượng)
void Game::update(float deltaTime)
{
    // --- XỬ LÝ LOGIC (Theo Trạng Thái) --- (Lấy từ main() cũ)
    // (Đây là bộ não quản lý việc chuyển state tự động)
    switch (mCurrentState)
    {
    case TITLE_SCREEN:
        mTitleTimer -= deltaTime; // Đếm ngược
        if (mTitleTimer <= 0.f) { // Hết giờ
            mCurrentState = STARTING; // Tự động chuyển
            mStartingTimer = 2.0f;
        }
        break;

    case STARTING:
        mStartingTimer -= deltaTime;
        if (mStartingTimer <= 0.f) { // Hết giờ
            mCurrentState = PLAYING; // Bắt đầu chơi
            resetGame(); // Reset mọi thứ
            // (Đã xóa lệnh play nhạc ở đây, vì nhạc chạy từ Menu)
        }
        break;

    case GAME_OVER:
        // Đảm bảo nhạc/âm thanh chỉ chạy 1 lần khi state vừa chuyển
        // (Dành cho trường hợp chết do hết máu, thay vì nhấn Enter)
        if (mBackgroundMusic.getStatus() == sf::Music::Playing)
        {
            mBackgroundMusic.stop();
            mGameOverSound.play();
        }

        mGameOverTimer -= deltaTime; // Đếm ngược
        if (mGameOverTimer <= 0.f) { // Hết giờ
            mCurrentState = SCORE_SCREEN; // Chuyển sang màn hình điểm

            // Xử lý điểm cao (Lấy từ main() cũ)
            mHighScores.push_back(mPlayer.getScore()); // Thêm điểm mới
            std::sort(mHighScores.rbegin(), mHighScores.rend()); // Sắp xếp giảm dần
            if (mHighScores.size() > 10) { mHighScores.resize(10); } // Giữ lại top 10
            saveHighScores(); // Lưu ra file

            // Yêu cầu UI chuẩn bị text cho màn hình điểm
            mUI.setupScoreScreen(mHighScores, mPlayer.getScore());
        }
        break;

    case PLAYING:
        updatePlaying(deltaTime); // Gọi hàm logic chơi game riêng
        break;

    case MENU:
    case SCORE_SCREEN:
        // Không có logic update động
        break;
    }
}

// Logic chính khi đang ở state PLAYING
void Game::updatePlaying(float deltaTime)
{
    // *** ĐÂY LÀ TRÁI TIM CỦA GAME ***
    // (Các đối tượng tương tác với nhau)

    // 1. Cập nhật Player (vật lý, di chuyển, animation)
    // (Truyền platform từ Map cho Player biết đường va chạm)
    mPlayer.update(deltaTime, mMap.getPlatforms());

    // 2. Cập nhật Item (va chạm, di chuyển)
    // (Truyền Player cho Item biết để kiểm tra va chạm và "hút" item)
    mItem.update(deltaTime, mPlayer);

    // 3. Cập nhật UI (điểm, xu, tim)
    // (Lấy thông tin mới nhất từ Player)
    mUI.update(mPlayer.getScore(), mPlayer.getCoins());
    mUI.updateHearts(mPlayer.getLives());

    // 4. Cập nhật Camera đi theo Player
    updateCamera();

    // 5. Kiểm tra nếu Player chết
    if (mPlayer.isDead() && mCurrentState == PLAYING) // (Phải check state để tránh gọi 2 lần)
    {
        mCurrentState = GAME_OVER; // Chuyển state
        mGameOverTimer = 5.0f; // Đặt giờ cho màn hình "GAME OVER"
    }
}

// Vẽ mọi thứ
void Game::render()
{
    // --- XỬ LÝ VẼ (Theo Trạng Thái) --- (Lấy từ main() cũ)

    // Xóa màn hình
    // (Các state như Menu, Title, Game Over... sẽ tự clear màn hình)
    if (mCurrentState == PLAYING)
    {
        mWindow.clear(sf::Color(92, 148, 252)); // Xóa bằng màu trời
    }

    // 1. Vẽ Mây (Parallax) - (Chỉ khi đang chơi)
    if (mCurrentState == PLAYING)
    {
        // (Truyền cả camera mây và camera chính để tính toán parallax)
        mMap.renderParallax(mWindow, mCloudView, mCamera);
    }

    // 2. Vẽ Thế Giới Game (Platform, Item, Player) - (Chỉ khi đang chơi)
    if (mCurrentState == PLAYING)
    {
        mWindow.setView(mCamera); // BẬT camera chính (di chuyển theo player)
        mMap.render(mWindow);
        mItem.render(mWindow);
        mPlayer.render(mWindow);
        // (Thứ tự vẽ: Map -> Item -> Player để Player luôn ở trên cùng)
    }

    // 3. Vẽ UI (Luôn vẽ đè lên trên cùng)
    // (UI::render sẽ tự TẮT camera chính (setView(Default)) và vẽ đúng thứ)
    mUI.render(mWindow, mCurrentState);

    // Hiển thị tất cả những gì đã vẽ ra cửa sổ
    mWindow.display();
}

// Reset tất cả các đối tượng về ban đầu
void Game::resetGame()
{
    // --- GỌI HÀM RESET CỦA TỪNG ĐỐI TƯỢNG ---
    mPlayer.reset();
    mMap.reset();
    mItem.reset(mMap.getPlatforms()); // Item cần biết platform ở đâu để sinh
    mUI.reset(mPlayer.getLives()); // UI cần biết số mạng ban đầu để vẽ tim
}

// Cập nhật camera đi theo Player
void Game::updateCamera()
{
    // --- CẬP NHẬT CAMERA --- (Lấy từ updatePlaying() cũ)
    float cameraX = mPlayer.getCenter().x;
    float cameraY = mPlayer.getCenter().y;

    // Giới hạn camera (không cho đi quá màn hình)
    float cameraYCenterMin = WINDOW_HEIGHT / 2.f; float cameraYPosMin = GAME_HEIGHT - cameraYCenterMin;
    if (cameraY > cameraYPosMin) cameraY = cameraYPosMin; // Giới hạn dưới
    float cameraXCenterMin = WINDOW_WIDTH / 2.f; float cameraXPosMin = cameraXCenterMin; float cameraXMax = GAME_WIDTH - cameraXCenterMin;
    if (cameraX < cameraXPosMin) cameraX = cameraXPosMin; // Giới hạn trái
    if (cameraX > cameraXMax) cameraX = cameraXMax; // Giới hạn phải
    mCamera.setCenter(cameraX, cameraY); // Đặt vị trí mới cho camera
}

// --- CÁC HÀM TIỆN ÍCH FILE (Đọc/Lưu điểm cao) --- (Lấy từ file gốc)

void Game::loadHighScores() {
    mHighScores.clear();
    std::ifstream inputFile;
    try {
        inputFile.open(HIGHSCORE_FILE); // Mở file
        inputFile.exceptions(std::ifstream::failbit | std::ifstream::badbit); // Bật ngoại lệ
        int score;
        while (inputFile >> score) { // Đọc từng số
            mHighScores.push_back(score);
        }
        inputFile.close();
        std::sort(mHighScores.rbegin(), mHighScores.rend()); // Sắp xếp giảm dần
    }
    catch (const std::exception& e) {
        // (Không in lỗi nếu file không tồn tại)
        if (inputFile.is_open()) {
            inputFile.close();
        }
    }
}

void Game::saveHighScores() {
    std::ofstream outputFile;
    try {
        outputFile.open(HIGHSCORE_FILE); // Mở file (sẽ ghi đè)
        outputFile.exceptions(std::ofstream::failbit | std::ofstream::badbit); // Bật ngoại lệ
        for (int score : mHighScores) {
            outputFile << score << std::endl; // Ghi từng điểm
        }
        outputFile.close();
    }
    catch (const std::exception& e) {
        std::cerr << "Loi khi luu file diem cao: " << e.what() << std::endl;
        if (outputFile.is_open()) {
            outputFile.close();
        }
    }
}