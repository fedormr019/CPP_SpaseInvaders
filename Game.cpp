#include "Game.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <string>

Game::Game() : win(nullptr), ren(nullptr), running(false), gameOver(false),
player(Vec2(400, 550), 800), screenW(800), screenH(600), level(1), enemyBaseShootDelay(1.0),
enemyMoveTimer(0), enemyMoveDelay(0.5), enemyBaseMoveDelay(0.5), enemyShootTimer(0), enemyShootDelay(1.0), enemyDirX(1), font(nullptr) {
}

Game::~Game() {
    shutdown();
}

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL error: " << SDL_GetError() << std::endl;
        return false;
    }

    win = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        screenW, screenH, SDL_WINDOW_SHOWN);
    if (!win) {
        std::cout << "window error: " << SDL_GetError() << std::endl;
        return false;
    }

    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (!ren) {
        std::cout << "renderer error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() < 0) {
        std::cout << "TTF error: " << TTF_GetError() << std::endl;
        return false;
    }

    font = TTF_OpenFont("arial.ttf", 24);
    if (!font) {
        std::cout << "font error: " << TTF_GetError() << std::endl;
        return false;
    }

    srand(time(nullptr));
    loadLevel(level);
    running = true;
    return true;
}

void Game::loadLevel(int lvl) {
    if (lvl > 3) {
        running = false;
        return;
    }
    enemies.clear();
    bullets.clear();
    player.setPos(Vec2(screenW / 2 - 20, screenH - 70));
    player.setActive(true);

    std::string filename = "level" + std::to_string(lvl) + ".txt";

    std::ifstream file(filename);

    int rows, cols;
    file >> rows >> cols;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            char ch;
            file >> ch;
            if (ch == '1') {
                Vec2 pos(50 + c * 50, 50 + r * 40);
                enemies.push_back(Enemy(pos));
            }
        }
    }

    std::string key;
    while (file >> key) {
        if (key == "move_delay") {
            file >> enemyMoveDelay;
            enemyBaseMoveDelay = enemyMoveDelay;
        }
        else if (key == "shoot_delay") {
            file >> enemyShootDelay;
            enemyBaseShootDelay = enemyShootDelay;
        }
    }

    file.close();
}

void Game::drawUI() {
    SDL_Color color = { 255, 255, 255, 255 };

    std::string livesText = "Lives: " + std::to_string(player.getLives());
    std::string scoreText = "Score: " + std::to_string(player.getScore());

    SDL_Surface* livesSurf = TTF_RenderUTF8_Blended(font, livesText.c_str(), color);
    SDL_Surface* scoreSurf = TTF_RenderUTF8_Blended(font, scoreText.c_str(), color);

    if (livesSurf) {
        SDL_Texture* livesTex = SDL_CreateTextureFromSurface(ren, livesSurf);
        SDL_Rect livesRect = { 10, 10, livesSurf->w, livesSurf->h };
        SDL_RenderCopy(ren, livesTex, NULL, &livesRect);
        SDL_DestroyTexture(livesTex);
        SDL_FreeSurface(livesSurf);
    }

    if (scoreSurf) {
        SDL_Texture* scoreTex = SDL_CreateTextureFromSurface(ren, scoreSurf);
        SDL_Rect scoreRect = { screenW - scoreSurf->w - 10, 10, scoreSurf->w, scoreSurf->h };
        SDL_RenderCopy(ren, scoreTex, NULL, &scoreRect);
        SDL_DestroyTexture(scoreTex);
        SDL_FreeSurface(scoreSurf);
    }
}

void Game::updateEnemies(double dt) {
    if (enemies.empty()) return;

    enemyMoveTimer += dt;
    if (enemyMoveTimer >= enemyMoveDelay) {
        enemyMoveTimer = 0;

        bool moveDown = false;

        for (auto& enemy : enemies) {
            Vec2 newPos = enemy.getPos() + Vec2(20 * enemyDirX, 0);
            if (newPos.x + enemy.getWidth() > screenW || newPos.x < 0) {
                moveDown = true;
                break;
            }
        }

        if (moveDown) {
            for (auto& enemy : enemies) {
                enemy.setPos(enemy.getPos() + Vec2(0, 20));

                if (enemy.getPos().y + enemy.getHeight() >= player.getPos().y) {
                    gameOver = true;
                }
            }
            enemyDirX = -enemyDirX;
        }
        else {
            for (auto& enemy : enemies) {
                enemy.setPos(enemy.getPos() + Vec2(20 * enemyDirX, 0));
            }
        }
    }
}

void Game::updateBullets(double dt) {
    for (auto& bullet : bullets) {
        bullet.update(dt);
    }
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [](const Bullet& b) { return !b.isActive(); }), bullets.end());
}

void Game::checkCollisions() {
    for (auto& bullet : bullets) {
        if (bullet.isFromPlayer()) {
            for (auto& enemy : enemies) {
                if (bullet.getRect().x < enemy.getRect().x + enemy.getRect().w &&
                    bullet.getRect().x + bullet.getRect().w > enemy.getRect().x &&
                    bullet.getRect().y < enemy.getRect().y + enemy.getRect().h &&
                    bullet.getRect().y + bullet.getRect().h > enemy.getRect().y)
                {
                    bullet.setActive(false);
                    enemy.setActive(false);
                    player.addScore(10);

                    enemyMoveDelay *= 0.98;
                    if (enemyMoveDelay < 0.1) enemyMoveDelay = 0.1;

                    break;
                }
            }
        }
        else {
            if (bullet.getRect().x < player.getRect().x + player.getRect().w &&
                bullet.getRect().x + bullet.getRect().w > player.getRect().x &&
                bullet.getRect().y < player.getRect().y + player.getRect().h &&
                bullet.getRect().y + bullet.getRect().h > player.getRect().y)
            {
                bullet.setActive(false);
                player.hit();
                if (player.getLives() <= 0) {
                    gameOver = true;
                }
            }
        }
    }

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const Enemy& e) { return !e.isActive(); }), enemies.end());
}

bool Game::allEnemiesDead() {
    return enemies.empty();
}

void Game::nextLevel() {
    level++;
    player.addScore(100);
    player.addLife();

    loadLevel(level);
}

void Game::processInput() {
    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (gameOver) {
        if (keys[SDL_SCANCODE_SPACE] || keys[SDL_SCANCODE_RETURN] || keys[SDL_SCANCODE_ESCAPE]) {
            running = false;
        }
        return;
    }

    if (keys[SDL_SCANCODE_LEFT]) {
        player.moveLeft(1.0f / 60.0f);
    }
    if (keys[SDL_SCANCODE_RIGHT]) {
        player.moveRight(1.0f / 60.0f);
    }
    if (keys[SDL_SCANCODE_SPACE] && player.canShoot()) {
        Vec2 pos(player.getPos().x + player.getWidth() / 2, player.getPos().y);
        Vec2 vel(0, -500);
        bullets.push_back(Bullet(pos, vel, true, screenH));
        player.resetShootTimer();
    }
}

void Game::update(double dt) {
    if (gameOver) {
        return;
    }
    player.update(dt);
    updateBullets(dt);
    updateEnemies(dt);
    checkCollisions();

    enemyShootTimer += dt;
    if (enemyShootTimer >= enemyShootDelay) {
        enemyShootTimer = 0;
        if (!enemies.empty()) {
            int idx = rand() % enemies.size();
            Vec2 pos = enemies[idx].getPos();
            pos.x += enemies[idx].getWidth() / 2;
            pos.y += enemies[idx].getHeight();
            Vec2 vel(0, 300);
            bullets.push_back(Bullet(pos, vel, false, screenH));
        }
    }

    if (allEnemiesDead()) {
        nextLevel();
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderClear(ren);

    if (gameOver) {
        SDL_Color color = { 255, 0, 0, 255 };
        SDL_Surface* surf = TTF_RenderUTF8_Blended(font, "GAME OVER", color);
        if (surf) {
            SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, surf);
            SDL_Rect dst = { screenW / 2 - surf->w / 2, screenH / 2 - surf->h / 2, surf->w, surf->h };
            SDL_RenderCopy(ren, tex, NULL, &dst);
            SDL_DestroyTexture(tex);
            SDL_FreeSurface(surf);
        }
        SDL_RenderPresent(ren);
        return;
    }

    player.draw(ren);

    for (auto& enemy : enemies) {
        enemy.draw(ren);
    }

    for (auto& bullet : bullets) {
        bullet.draw(ren);
    }

    drawUI();

    SDL_RenderPresent(ren);
}

void Game::run() {
    Uint32 lastTime = SDL_GetTicks();

    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        double dt = (currentTime - lastTime) / 1000.0;
        if (dt > 0.033) dt = 0.033;
        lastTime = currentTime;

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
        }

        processInput();
        update(dt);
        render();
        SDL_Delay(16);
    }
}

void Game::shutdown() {
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }
    if (ren) SDL_DestroyRenderer(ren);
    if (win) SDL_DestroyWindow(win);
    TTF_Quit();
    SDL_Quit();
}