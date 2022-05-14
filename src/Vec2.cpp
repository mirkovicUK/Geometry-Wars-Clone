#include "Vec2.h"
#include <math.h>




Vec2::Vec2() {}

Vec2::Vec2(float xin, float yin)
    : x(xin), y(yin) {}

// operator works on 2 vectors
Vec2 Vec2::operator +(const Vec2 & rhs) const    {return Vec2(x + rhs.x, y + rhs.y);}
Vec2 Vec2::operator -(const Vec2 & rhs) const    {return Vec2(x - rhs.x, y - rhs.y);}

// vector and float val
Vec2 Vec2::operator / (const float val) const    {return Vec2(x / val, y / val);}
Vec2 Vec2::operator * (const float val ) const   {return Vec2(x * val, y * val);}

// operator on 2 vectors
bool Vec2::operator == (const Vec2 & rhs) const  {return ((x == rhs.x) && (y == rhs.y));}
bool Vec2::operator != (const Vec2 & rhs) const  {return ((x != rhs.x) && (y != rhs.y));}

// on 2 vectors
void Vec2::operator += (const Vec2 & rhs)        { x += rhs.x;  y += rhs.y; }
void Vec2::operator -= (const Vec2 & rhs)        { x -= rhs.x; y -= rhs.y; }

// vector and float val
void Vec2::operator /= (const float val )        { x /= val; y /= val; } 
void Vec2::operator *= (const float val )        { x *= val; y *= val; }

// dist betwen 2d points
float Vec2::dist(const Vec2 & rhs) const 
{
    return (float)sqrtf((rhs.x - x)*(rhs.x - x) + (rhs.y - y)*(rhs.y - y));
}

std::ostream& operator << (std::ostream& os , const Vec2 & vec2)
{
    return os << "(" <<vec2.x<<","<<vec2.y<<")";
} 
