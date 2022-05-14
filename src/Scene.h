
#pragma once

#include "Common.h"
#include "Action.h"
#include "Action2.h"
#include "EntityManager.h"

#include <memory>

class GameEngine;

typedef std::map<int, std::string> ActionMap;

class Scene
{

protected:

    GameEngine *    m_game = nullptr;  
    EntityManager   m_entities;
    ActionMap       m_actionMap;
    bool            m_paused   = false;
    bool            m_hasEnded = false;
    std::size_t     m_currentFrame = 0;

public:

    Scene();
    Scene(GameEngine * gameEngine);

    virtual void update() = 0;
    virtual void sDoAction(const Action & action) = 0;
    virtual void sRender() = 0;
    virtual void onEnd() = 0;
    
    void registerAction(int keyCode, std::string ActionName);
    void doAction (const Action & action);
    void setPaused();

    const ActionMap & getActionMap() const;
    const EntityManager& getEntityManager() const;
    
    
};