#include "Action2.h"

 Action2::Action2 () {}
 Action2::Action2 (const int & posX, const int & posY, const std::string & name, const std::string & type)
    :Action(name, type), positionX(posX), positionY(posY) {}

const int & Action2::getPositionX() const
{
    return positionX;
}
const int & Action2::getPositionY() const
{
    return positionY;
}