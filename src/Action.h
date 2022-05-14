#pragma once

#include "Common.h"

class Action
{
    std::string m_name = "none"; //action like shoot jump etc
    std::string m_type = "none"; // start or end
    

public:
    Action();
    Action(const std::string & name, const std::string & type);
    const std::string & name() const;    
    const std::string & type() const;

    virtual void dummy(void) const;    
};