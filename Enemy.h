#pragma once
#include "GameObject.h"

class Enemy : public GameObject {
public:
    Enemy(const Vec2& p);
    void update(double dt) override;
    void draw(SDL_Renderer* ren) const override;
};