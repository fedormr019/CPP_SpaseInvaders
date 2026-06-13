#include "Enemy.h"

Enemy::Enemy(const Vec2& p) : GameObject(p, 30, 30) {}

void Enemy::update(double dt) {}

void Enemy::draw(SDL_Renderer* ren) const {
    SDL_Rect rect = getRect();
    SDL_SetRenderDrawColor(ren, 255, 0, 255, 255);
    SDL_RenderFillRect(ren, &rect);
}