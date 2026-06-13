#include "Player.h"
#include <iostream>

Player::Player(const Vec2& p, int screenWidth)
    : GameObject(p, 40, 20), lives(3), score(0), shootTimer(0), shootDelay(0.3f), screenW(screenWidth) {
}

void Player::update(double dt) {
    shootTimer += dt;
}

void Player::draw(SDL_Renderer* ren) const {
    SDL_Rect rect = getRect();
    SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
    SDL_RenderFillRect(ren, &rect);
}

void Player::moveLeft(double dt) {
    pos.x -= 400 * dt;
    if (pos.x < 0) pos.x = 0;
}

void Player::moveRight(double dt) {
    pos.x += 400 * dt;
    if (pos.x + w > screenW) pos.x = screenW - w;
}

bool Player::canShoot() {
    return shootTimer >= shootDelay;
}

void Player::resetShootTimer() {
    shootTimer = 0;
}

void Player::hit() {
    lives--;
    if (lives <= 0) {
        active = false;
    }
}

void Player::addScore(int pts) {
    score += pts;
}

void Player::addLife() {
    lives++;
}

int Player::getLives() const {
    return lives;
}

int Player::getScore() const {
    return score;
}

int Player::getScreenWidth() const {
    return screenW;
}