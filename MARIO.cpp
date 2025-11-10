// *** DÒNG NÀY CỰC KỲ QUAN TRỌNG! ***
#include "Game.h" // Chỉ cần include "nhạc trưởng"

int main()
{
    // Khởi tạo seed ngẫu nhiên (chỉ chạy 1 lần)
    // Dùng để các hàm rand() ra kết quả khác nhau mỗi lần chạy
    srand(static_cast<unsigned>(time(0)));

    // 1. Tạo đối tượng Game
    // (Hàm khởi tạo Game::Game() sẽ được gọi, tải tất cả assets, bật nhạc nền)
    Game game;

    // 2. Bắt đầu vòng lặp game
    // (Chương trình sẽ chạy bên trong hàm này cho đến khi cửa sổ đóng)
    game.run();

    return 0; // Kết thúc chương trình
}