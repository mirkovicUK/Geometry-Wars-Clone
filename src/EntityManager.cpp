#include "EntityManager.h"

EntityManager::EntityManager()
     {}


void EntityManager::update()
{
    for (auto  e : m_entitiesToAdd)  
    {
        m_entities.push_back(e);
        m_entityMap[e->tag()].push_back(e); 
    }
    m_entitiesToAdd.clear();
    
    removeDeadEntities(m_entities);
    
    // remove dead from map
    for (auto & pair : m_entityMap)
    {
        removeDeadEntities(pair.second); 
    }
}

void EntityManager::removeDeadEntities(EntityVec & vec)
{

 vec.erase(std::remove_if(vec.begin(), vec.end(),
          [=] ( auto entity) { return !(entity->isActive()); })  ,vec.end());

}

// create / alocate new Entity and add it to vector m_entitiesToAdd 
 std::shared_ptr<Entity> EntityManager::addEntity(const std::string & tag)
{
   auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));   

    m_entitiesToAdd.push_back(entity);

    return  entity;
}

const EntityVec & EntityManager::getEntities()
{
    return m_entities;
}

const EntityVec & EntityManager::getEntities(const std::string & tag)
{
    for (const auto & e : m_entityMap)
    {
        if (e.first == tag)
        {
            return e.second;
        }
    }
    static const EntityVec dummy; 
    return dummy;
}




//deep copy assignment operator (pointers in vector, pointers in Entity)
// still missing some components values 
EntityManager& EntityManager::operator = (const EntityManager & object)
{
    std::cout<<"Start deep copy"<<std::endl;
    if (!object.m_entities.empty()) // check for empty rhs vec
    {
    m_entities.clear();//clear lhs b4 coping to avoid double copies
    for (auto p : object.m_entities) 
    {
        m_entities.push_back(std::shared_ptr<Entity>(new Entity(p->m_id, p->m_tag))); //create entity 
        m_entities.back()->m_active = p->m_active;//copy m_members that are out of constructor

        //check for active components in entity and copy them to new entity
        if (!m_entities.empty()) 
        {
            if (p->cTransform)
            {
                m_entities.back()->cTransform = 
                std::make_shared<CTransform>(p->cTransform->pos, p->cTransform->velocity, p->cTransform->angle);
            }

            if (p->cShape)
            {
                m_entities.back()->cShape = std::make_shared<CShape>(p->cShape->circle.getRadius(),
                p->cShape->circle.getPointCount(), p->cShape->circle.getFillColor(), p->cShape->circle.getOutlineColor(),
                p->cShape->circle.getOutlineThickness());
            }

            if (p->cCollision)
            {
                m_entities.back()->cCollision = std::make_shared<CCollision>(p->cCollision->radious);
                //m_entities.back()->cCollision->radious = p->cCollision->radious;
            }

            if (p->cScore)
            {
                m_entities.back() -> cScore = std::make_shared<CScore>(p->cScore->score);
            }

            if (p->cLifespan)
            {
                m_entities.back()->cLifespan = std::make_shared<CLifespan>(p->cLifespan->total);
                m_entities.back()->cLifespan->remaining = p->cLifespan->remaining; 
            }

            if (p->cInput)
            {
                m_entities.back()->cInput = std::make_shared<CInput>(); // creating CInput object
                
                //coping objects vriables 
                m_entities.back()->cInput->down         = p->cInput->down;
                m_entities.back()->cInput->inWinXleft   = p->cInput->inWinXleft;
                m_entities.back()->cInput->inWinXright  = p->cInput->inWinXright;
                m_entities.back()->cInput->inWinYdown   = p->cInput->inWinYdown;
                m_entities.back()->cInput->inWinYup     = p->cInput->inWinYup; 
                m_entities.back()->cInput->left         = p->cInput->left;
                m_entities.back()->cInput->right        = p->cInput->right;
                m_entities.back()->cInput->shoot        = p->cInput->shoot;
                m_entities.back()->cInput->up           = p->cInput->up;
           }

           if (p->cSpecialWeapon)
           {
               m_entities.back()->cSpecialWeapon = std::make_shared<CSpecialWeapon>(p->cSpecialWeapon->reloadTime);
               m_entities.back()->cSpecialWeapon->active = p->cSpecialWeapon->active;
               m_entities.back()->cSpecialWeapon->count  = p->cSpecialWeapon->count;
               m_entities.back()->cSpecialWeapon->countdown  = p->cSpecialWeapon->countdown;
               m_entities.back()->cSpecialWeapon->reload  = p->cSpecialWeapon->reload;
           }


        }
        else
        {
            std::cout<< "Faild to create Entity line 116"<< std::endl;
        }
    }
    }
    else
    {
        std::cout<<"rhs m_entities is EMPTY"<< std::endl;
        m_entities = object.m_entities;
    }


    std::cout<<"Start deep copy : 2nd loop"<<std::endl;

    //check is rhs vec is not empty
    if (!object.m_entitiesToAdd.empty())
    {
        m_entitiesToAdd.clear();
        for (auto p : object.m_entitiesToAdd)
        { 
            m_entitiesToAdd.push_back(std::shared_ptr<Entity>(new Entity(p->m_id, p->m_tag))); 
            m_entitiesToAdd.back()->m_active = p->m_active;

                //check for active components in entity and copy them
            if (!m_entitiesToAdd.empty()) 
            {

                if (p->cTransform)
                {
                    m_entitiesToAdd.back()->cTransform = 
                    std::make_shared<CTransform>(p->cTransform->pos, p->cTransform->velocity, p->cTransform->angle);
                }

                if (p->cShape)
                {
                    m_entitiesToAdd.back()->cShape = std::make_shared<CShape>(p->cShape->circle.getRadius(),
                    p->cShape->circle.getPointCount(), p->cShape->circle.getFillColor(), p->cShape->circle.getOutlineColor(),
                    p->cShape->circle.getOutlineThickness());
                }

                if (p->cCollision)
                {
                    m_entitiesToAdd.back()->cCollision = std::make_shared<CCollision>(p->cCollision->radious);
                    //m_entitiesToAdd.back()->cCollision->radious = p->cCollision->radious;
                }

                if (p->cScore)
                {
                    m_entitiesToAdd.back() -> cScore = std::make_shared<CScore>(p->cScore->score);
                }

                if (p->cLifespan)
                {
                    m_entitiesToAdd.back()->cLifespan = std::make_shared<CLifespan>(p->cLifespan->total);
                    m_entitiesToAdd.back()->cLifespan->remaining = p->cLifespan->remaining; 
                }

                if (p->cInput)
                {
                    m_entitiesToAdd.back()->cInput = std::make_shared<CInput>(); // creating CInput object
                
                    //coping values
                    m_entitiesToAdd.back()->cInput->down        = p->cInput->down;
                    m_entitiesToAdd.back()->cInput->inWinXleft  = p->cInput->inWinXleft;
                    m_entitiesToAdd.back()->cInput->inWinXright = p->cInput->inWinXright;
                    m_entitiesToAdd.back()->cInput->inWinYdown  = p->cInput->inWinYdown;
                    m_entitiesToAdd.back()->cInput->inWinYup    = p->cInput->inWinYup; 
                    m_entitiesToAdd.back()->cInput->left        = p->cInput->left;
                    m_entitiesToAdd.back()->cInput->right       = p->cInput->right;
                    m_entitiesToAdd.back()->cInput->shoot       = p->cInput->shoot;
                    m_entitiesToAdd.back()->cInput->up          = p->cInput->up;
                }

                if (p->cSpecialWeapon)
                {
                    m_entitiesToAdd.back()->cSpecialWeapon = std::make_shared<CSpecialWeapon>(p->cSpecialWeapon->reloadTime);
                    m_entitiesToAdd.back()->cSpecialWeapon->active = p->cSpecialWeapon->active;
                    m_entitiesToAdd.back()->cSpecialWeapon->count  = p->cSpecialWeapon->count;
                    m_entitiesToAdd.back()->cSpecialWeapon->countdown  = p->cSpecialWeapon->countdown;
                    m_entitiesToAdd.back()->cSpecialWeapon->reload  = p->cSpecialWeapon->reload;
                }
            }
            else
            {
                std::cout<< "Faild to create Entity line 198"<< std::endl;
            }
        }
    }
    else
    {
        std::cout<<"rhs m_entitiesToAdd is empty" << std::endl;
        m_entitiesToAdd = object.m_entitiesToAdd;
    }
    

    std::cout<<"Start deep copy : 3rd loop"<<std::endl;
    //check if rhs map is empty
    if (!object.m_entityMap.empty())
    {
        m_entityMap.clear();
        std::cout<<"Check Point1"<<std::endl;
        for (auto p : object.m_entityMap)//loop throught rhs map
        {
            std::cout<<"Check Point2"<<std::endl;
            EntityVec vector;
            if (!p.second.empty()) 
            {
                for (auto v : p.second) //looping throught Entity vector 
                {
                    std::cout<<"Check Point3"<<std::endl;
                    vector.push_back(std::shared_ptr<Entity>(new Entity(v->m_id, v->m_tag))); //creating entity
                    vector.back()->m_active = v->m_active;//coping variables
                    std::cout<<"Check Point4"<<std::endl;

                    if (!vector.empty()) 
                    {
                        std::cout<<"Check Point5"<<std::endl;
                        if (v->cTransform)
                        {
                            vector.back()->cTransform = 
                            std::make_shared<CTransform>(v->cTransform->pos, v->cTransform->velocity, v->cTransform->angle);
                        }

                        std::cout<<"Check Point6"<<std::endl;
                        if (v->cShape)
                        {
                            vector.back()->cShape = std::make_shared<CShape>(v->cShape->circle.getRadius(),
                            v->cShape->circle.getPointCount(), v->cShape->circle.getFillColor(), v->cShape->circle.getOutlineColor(),
                            v->cShape->circle.getOutlineThickness());
                        }
                        
                        std::cout<<"Check Point7"<<std::endl;
                        if (v->cCollision)
                        {
                            vector.back()->cCollision = std::make_shared<CCollision>(v->cCollision->radious);
                            //vector.back()->cCollision->radious = v->cCollision->radious;
                        }

                        std::cout<<"Check Point8"<<std::endl;
                        if (v->cScore)
                        {
                            vector.back() -> cScore = std::make_shared<CScore>(v->cScore->score);
                        }

                        std::cout<<"Check Point9"<<std::endl;
                        if (v->cLifespan)
                        {
                            vector.back()->cLifespan = std::make_shared<CLifespan>(v->cLifespan->total);
                            vector.back()->cLifespan->remaining = v->cLifespan->remaining; 
                        }

                        std::cout<<"Check Point10"<<std::endl;
                        if (v->cInput)
                        {
                            vector.back()->cInput = std::make_shared<CInput>(); // creating CInput object
                    
                            //coping values
                            vector.back()->cInput->down        = v->cInput->down;
                            vector.back()->cInput->inWinXleft  = v->cInput->inWinXleft;
                            vector.back()->cInput->inWinXright = v->cInput->inWinXright;
                            vector.back()->cInput->inWinYdown  = v->cInput->inWinYdown;
                            vector.back()->cInput->inWinYup    = v->cInput->inWinYup; 
                            vector.back()->cInput->left        = v->cInput->left;
                            vector.back()->cInput->right       = v->cInput->right;
                            vector.back()->cInput->shoot       = v->cInput->shoot;
                            vector.back()->cInput->up          = v->cInput->up;
                        }

                        std::cout<<"Check Point11"<<std::endl;
                        if (v->cSpecialWeapon)
                        {
                            vector.back()->cSpecialWeapon = std::make_shared<CSpecialWeapon>(v->cSpecialWeapon->reloadTime);
                            vector.back()->cSpecialWeapon->active = v->cSpecialWeapon->active;
                            vector.back()->cSpecialWeapon->count  = v->cSpecialWeapon->count;
                            vector.back()->cSpecialWeapon->countdown  = v->cSpecialWeapon->countdown;
                            vector.back()->cSpecialWeapon->reload  = v->cSpecialWeapon->reload;
                        }
                    }
                    else
                    {
                        std::cout<< "Faild to create Entity line 283 EntityManager"<< std::endl;
                    }
                }
                std::cout<<"Check Point12"<<std::endl;
            }
            else
            {
                std::cout<<"rhs m_entityMap(.second) vector is empty "<<std::endl;
                vector = p.second;
            }
            
            m_entityMap[p.first] = vector;
        }
    }
    else
    {
        std::cout<<"rhs m_entityMap is Empty"<<std::endl;
        m_entityMap = object.m_entityMap;
    }
    

    std::cout<<" deep copy : last sizt_t copy"<<std::endl;
    m_totalEntities = object.m_totalEntities;
    counter = object.counter;

    std::cout<<"DEEP COPYING COMPLEAT"<<std::endl;

    filePrint (object);

    return *this;

}

//helper for assignment operator 
 void EntityManager::filePrint (const EntityManager & object)
{
    counter--;
    std::ofstream outdata;
    //std::stringstream ss;
    //ss << counter;
    //std::string string;
    //ss >> string;

    std::string name = "Uros";// + string;
    

    outdata.open(name, std::ios_base::app);
    if (!outdata)
    {
        std::cerr<<"Could not open file"<<std::endl;
        exit(1);
    }

    outdata<< "EntityManager Data printing :" << std::endl;
    outdata<< "LHS:m_entities.size(): " << m_entities.size()<<"  "<<"RHS:m_entities.size(): "<<object.m_entities.size()<<std::endl;
   
    outdata<< "LHS:m_entityToAdd.size(): "<<m_entitiesToAdd.size()<<"   "<<"RHS:m_entitiesToAdd.size(): "<< 
    object.m_entitiesToAdd.size() << std::endl;
    
    outdata<<"LHS:m_entityMap.size(): "<<m_entityMap.size()<<"  "<<"RHS:mm_entityMap.size():"<<
    object.m_entityMap.size()<<std::endl<<std::endl;

    outdata << "LHS:m_entities printing pointer addres" <<std::endl;
    for (auto p : m_entities)
    {
        if (p->cCollision)
        {
            outdata<<"cColision: "<<p->cCollision->radious<<std::endl;
        }

        if (p->cTransform)
                {
                    outdata<<"cTransform pos: "<< p->cTransform->pos.x << " " << p->cTransform->pos.y<<std::endl;
                    outdata<<"cTransform velocity: "<< p->cTransform->velocity.x << " " << p->cTransform->velocity.y<<std::endl;
                    outdata<<"cTransform angle: "<< p->cTransform->angle<<std::endl;
                }

         if (p->cShape)
                {
                    outdata<<"cShape radious: "<<p->cShape->circle.getRadius()<<std::endl;
                    outdata<<"cShape points: "<<p->cShape->circle.getPointCount()<<std::endl;
                }


        if (p->cScore)
                {
                    outdata << "cScore score: " <<p->cScore->score<<std::endl;
                }

        if (p->cLifespan)
                {
                    outdata << "cLifespan remaining: "<< p->cLifespan->remaining<<std::endl;
                    outdata << "cLifespan total: "<< p->cLifespan->total<<std::endl;
                }

    }

    outdata << "RHS:m_entities printing pointer addres" <<std::endl;
    for (auto p : object.m_entities)
    {
        if (p->cCollision)
        {
            outdata<<"cColision: "<<p->cCollision->radious<<std::endl;
        }

        if (p->cTransform)
                {
                    outdata<<"cTransform pos: "<< p->cTransform->pos.x << " " << p->cTransform->pos.y<<std::endl;
                    outdata<<"cTransform velocity: "<< p->cTransform->velocity.x << " " << p->cTransform->velocity.y<<std::endl;
                    outdata<<"cTransform angle: "<< p->cTransform->angle<<std::endl;
                }

         if (p->cShape)
                {
                    outdata<<"cShape radious: "<<p->cShape->circle.getRadius()<<std::endl;
                    outdata<<"cShape points: "<<p->cShape->circle.getPointCount()<<std::endl;
                }


        if (p->cScore)
                {
                    outdata << "cScore score: " <<p->cScore->score<<std::endl;
                }

        if (p->cLifespan)
                {
                    outdata << "cLifespan remaining: "<< p->cLifespan->remaining<<std::endl;
                    outdata << "cLifespan total: "<< p->cLifespan->total<<std::endl;
                }

    }

    
    outdata.close();

    /*if  (counter == 0)
    {
        exit(1);
    }*/

}