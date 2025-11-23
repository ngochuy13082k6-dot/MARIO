#pragma once 
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm> 
#include <cmath>     
#include <cstdlib>   
#include <ctime>     

// --- KÍCH THƯỚC GAME (2560x720) ---
const float WINDOW_WIDTH = 1280.f;
const float WINDOW_HEIGHT = 720.f;
const float GAME_WIDTH = WINDOW_WIDTH * 2; // Rộng 2560px
const float GAME_HEIGHT = WINDOW_HEIGHT;   // Cao 720px 

// --- VẬT LÝ & TỐC ĐỘ ---
const float GRAVITY = 0.6f;
const float MOVE_SPEED = 8.0f;
const float JUMP_POWER = -13.0f;
const float HOMING_SPEED = 6.0f;
const float PLAYER_HIT_DURATION = 0.5f; // Thời gian bất tử (I-frames)
const float PLAYER_POWERUP_DURATION = 1.0f;

// --- HOẠT ẢNH & SCALE ---
const int IDLE_FRAME_INDEX = 0;
const int FIRST_RUN_FRAME_INDEX = 1;
const int LAST_RUN_FRAME_INDEX = 3;
const float FRAME_WIDTH = 307.f;
const float FRAME_HEIGHT = 402.f;
const float ANIM_SPEED = 0.05f;

const float COIN_SCALE = 0.05f;
const float BOX_SCALE = 0.01f;
const float MUSHROOM_SCALE = 0.08f;
const float FIRE_FLOWER_SCALE = 0.08f;
const float HEART_SCALE = 0.05f;
const float HEART_SPACING = 5.f;
const float ADJUSTMENT_Y = 25.f;

// --- FILE IO ---
const std::string HIGHSCORE_FILE = "highscores.txt";

// --- ENUMS & STRUCTS ---
// Các trạng thái Game: PAUSED và VICTORY được thêm vào
enum GameState { MENU, TITLE_SCREEN, STARTING, PLAYING, PAUSED, VICTORY, GAME_OVER, SCORE_SCREEN };
enum RewardType { COIN, MUSHROOM, FIRE_FLOWER };

struct MysteryBox {
    sf::Sprite sprite;
    RewardType reward = COIN;
};
struct Reward {
    sf::Sprite sprite;
    RewardType type = COIN;
    sf::Vector2f velocity = { 0.f, 0.f };
    bool homing = true;
};