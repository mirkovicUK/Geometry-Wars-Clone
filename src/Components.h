#pragma once

#include "Common.h"

class CTransform
{
public:
    Vec2 pos       = { 0.0, 0.0 }; //positin indicate position of the center of object
    Vec2 velocity  = { 0.0, 0.0 };
    float angle    = 0;

    CTransform(const Vec2 & p, const Vec2 & v, float a)
        : pos(p), velocity(v), angle(a) {}
};

class CShape
{
public:
    sf::CircleShape circle;

    CShape(float radius, int points, const sf::Color & fill, const sf::Color & outline, float thickness)
        : circle(radius, points)
    {
        circle.setFillColor(fill);
        circle.setOutlineColor(outline);
        circle.setOutlineThickness(thickness);
        circle.setOrigin(radius, radius); // sets position of transform in middle centar
    }
};

class CCollision
{
public:
    float radious = 0;

    CCollision(float r)
        :radious(r) {}
};

class CScore
{
public:
    int score = 0;
    CScore(int s)
        : score(s) {}
};

class CLifespan
{
public:
    int remaining  = 0;
    int total      = 0;
    CLifespan(int total)
        : remaining(total), total(total) {}
};

// controling entity
class CInput
{
public:
    bool up    = false;
    bool left  = false;
    bool right = false;
    bool down  = false;
    bool shoot = false;
    bool inWinXleft  = true;
    bool inWinXright = true;
    bool inWinYup    = true;
    bool inWinYdown  = true;

    CInput() {}
};

class CSpecialWeapon
{
public:
    bool active = false;
    int  countdown = 0;
    int  count     = 0;
    bool reload = false;
    int  reloadTime = 0; //600fps = 10sec


    CSpecialWeapon(const int & reload)
        :reloadTime(reload) {}
};