#pragma once

#include "Common.h"
#include "Action.h"

//Class Action2 derive from Action adding 2 int values to Action object
// Use case : sf::Event::MouseButtonPressed,  event.mouseBottom.x
class Action2 : public Action
{
    int positionX = 0;
    int positionY = 0;
    
public:

    Action2 ();
    Action2 (const int & posX, const int & posY, const std::string & name, const std::string & type);
    const int & getPositionX() const;
    const int & getPositionY() const;
};