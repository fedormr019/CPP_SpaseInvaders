#pragma once
#include "GameObject.h"

class Player : public GameObject {
private:
    int lives;
    int score;
    float shootTimer;
    float shootDelay;
    int screenW;

public:
    Player(const Vec2& p, int screenWidth);
    void update(double dt) override;
    void draw(SDL_Renderer* ren) const override;

    void moveLeft(double dt);
    void moveRight(double dt);
    bool canShoot();
    void resetShootTimer();

    void hit();
    void addScore(int pts);
    void addLife();
    int getLives() const;
    int getScore() const;
    int getScreenWidth() const;
};
