#pragma once

#include "Common.h"
#include "Components.h"



class Entity
{
    friend class EntityManager;

    bool        m_active = true;
    std::size_t m_id     = 0;
    std::string m_tag    = "default";

    
    Entity(const std::size_t  id, const std::string & tag);
   
public: 
    // pointers to components
    std::shared_ptr<CTransform>     cTransform;
    std::shared_ptr<CShape>         cShape;
    std::shared_ptr<CCollision>     cCollision; 
    std::shared_ptr<CScore>         cScore;
    std::shared_ptr<CInput>         cInput;
    std::shared_ptr<CLifespan>      cLifespan;
    std::shared_ptr<CSpecialWeapon> cSpecialWeapon;

    // getters
    bool isActive() const;
    const std::string & tag() const;
    const std::size_t id() const;
    void destroy() ; 
};