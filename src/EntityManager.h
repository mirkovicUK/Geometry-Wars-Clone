#pragma once

#include "Common.h"
#include "Entity.h"

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map<std::string, EntityVec> EntityMap;

class EntityManager
{
    EntityVec m_entities;
    EntityVec m_entitiesToAdd;
    EntityMap m_entityMap;
    std::size_t m_totalEntities = 0;

    int counter = 200;

    void removeDeadEntities(EntityVec & vec);

public:

    EntityManager();
    void update();

    std::shared_ptr<Entity> addEntity(const std::string & tag);
   
    const EntityVec & getEntities();
    const EntityVec & getEntities(const std::string & tag);
    
    EntityManager& operator = (const EntityManager & object);
    void filePrint (const EntityManager & object);
};