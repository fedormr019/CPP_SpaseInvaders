#include "Bullet.h"

Bullet::Bullet(const Vec2& p, const Vec2& v, bool playerBullet, int screenHeight) : GameObject(p, 4, 8), fromPlayer(playerBullet), screenH(screenHeight) {
    vel = v;
}

void Bullet::update(double dt) {
    pos += vel * dt;
    if (isOutOfBounds()) {
        active = false;
    }
}

void Bullet::draw(SDL_Renderer* ren) const {
    SDL_Rect rect = getRect();
    if (fromPlayer) {
        SDL_SetRenderDrawColor(ren, 255, 255, 0, 255);
    }
    else {
        SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
    }
    SDL_RenderFillRect(ren, &rect);
}

bool Bullet::isFromPlayer() const {
    return fromPlayer;
}

bool Bullet::isOutOfBounds() const {
    return (pos.y + h < 0) || (pos.y > screenH);
}