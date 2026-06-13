#include "GameObject.h"

GameObject::GameObject() : pos(0, 0), vel(0, 0), w(0), h(0), active(true) {}

GameObject::GameObject(const Vec2& p, int width, int height)
    : pos(p), vel(0, 0), w(width), h(height), active(true) {
}

void GameObject::setActive(bool a) {
    active = a;
}

bool GameObject::isActive() const {
    return active;
}

Vec2 GameObject::getPos() const {
    return pos;
}

void GameObject::setPos(const Vec2& p) {
    pos = p;
}

int GameObject::getWidth() const {
    return w;
}

int GameObject::getHeight() const {
    return h;
}

void GameObject::setVelocity(const Vec2& v) {
    vel = v;
}

Vec2 GameObject::getVelocity() const {
    return vel;
}

SDL_Rect GameObject::getRect() const {
    SDL_Rect rect;
    rect.x = (int)pos.x;
    rect.y = (int)pos.y;
    rect.w = w;
    rect.h = h;
    return rect;
}
