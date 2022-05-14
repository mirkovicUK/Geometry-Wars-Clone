#include "Scene.h"

Scene::Scene() {}
Scene::Scene(GameEngine * gameEngine)
    :m_game(gameEngine) {}

void Scene::setPaused()
{
    m_paused = true;
}

void Scene::registerAction(int keyCode, std::string ActionName)
{
    m_actionMap[keyCode] = ActionName;
}

const ActionMap & Scene::getActionMap() const
{
    return m_actionMap;
}

void Scene::doAction (const Action & action)
{
    std::cout<<"Scene::doAction()"<<std::endl;

    sDoAction(action);
    
}

const EntityManager& Scene::getEntityManager() const
{
    return m_entities;
}

