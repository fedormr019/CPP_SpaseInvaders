#pragma once
#include "GameObject.h"

class Bullet : public GameObject {
private:
    bool fromPlayer;
    int screenH;

public:
    Bullet(const Vec2& p, const Vec2& v, bool playerBullet, int screenHeight);
    void update(double dt) override;
    void draw(SDL_Renderer* ren) const override;

    bool isFromPlayer() const;
    bool isOutOfBounds() const;
};