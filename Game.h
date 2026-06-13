#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"

class Game {
private:
    SDL_Window* win;
    SDL_Renderer* ren;
    bool running;

    Player player;
    std::vector<Enemy> enemies;
    std::vector<Bullet> bullets;

    double enemyMoveTimer;
    double enemyMoveDelay;
    double enemyShootTimer;
    double enemyShootDelay;
    double enemyBaseMoveDelay;
    double enemyBaseShootDelay;

    int screenW, screenH;
    int level;
    int enemyDirX;

    void loadLevel(int lvl);
    void updateEnemies(double dt);
    void updateBullets(double dt);
    void checkCollisions();
    bool allEnemiesDead();
    void nextLevel();
    bool gameOver;

    void processInput();
    void update(double dt);
    void render();
    void drawUI();

    TTF_Font* font;

public:
    Game();
    ~Game();
    bool init();
    void run();
    void shutdown();
};