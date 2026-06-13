#pragma once
#include "Vec2.h"
#include <SDL.h>

class GameObject {
protected:
    Vec2 pos;
    Vec2 vel;
    int w, h;
    bool active;

public:
    GameObject();
    GameObject(const Vec2& p, int width, int height);
    virtual ~GameObject() = default;

    virtual void update(double dt) = 0;
    virtual void draw(SDL_Renderer* ren) const = 0;

    void setActive(bool a);
    bool isActive() const;

    Vec2 getPos() const;
    void setPos(const Vec2& p);

    int getWidth() const;
    int getHeight() const;

    void setVelocity(const Vec2& v);
    Vec2 getVelocity() const;

    SDL_Rect getRect() const;
};