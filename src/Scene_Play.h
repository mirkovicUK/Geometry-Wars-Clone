#pragma once

#include "Common.h"
#include "Scene.h"
#include <map>
#include <memory>
#include "Action.h"

#include "EntityManager.h"

class Scene_Play : public Scene
{
protected:

    std::shared_ptr<Entity> m_player;     //store separet reference to the player 

    int               m_lastEnemySpawnTime = 0;
    int               m_lifeCounter  = 3;
    int               m_score = 0;
    sf::Font          m_font ;     // font we will use to draw to the screen
    sf::Text          m_text;      // score text to be drawn to the screen
    sf::Text          m_life;      // draw lifes
    sf::Text          m_specialWepon; //draw special wepon info

    sf::Texture       m_texture;
    sf::Sprite        m_sprite;
    
    

    void init();
    
    void update()  override;
    void onEnd()   override;
    void sRender() override;
    void sDoAction(const Action & action) override;
    //void sDoAction(const Action2 & action2);

    void  spawnPlayer();
    void  spawnEnemy ();
    void  spawnBullet(std::shared_ptr<Entity> entity, const Vec2 & target);
    int   randomiser (int min, int max);
    float randomiser (float min, float max);
    void  spawnSmallEnemies(std::shared_ptr<Entity> e);
    void  setText();
    void  calcScore(const std::shared_ptr<Entity> entity);
    void  calcLife (void);
    void  setBackground();
    void SpecialWeaponControl(std::shared_ptr<Entity> entity);
    void spawnSpecialWeapon  (std::shared_ptr<Entity> entity);
    void sMovement();
    void sCollision();
    void sEnemySpawner();
    void sLifespan();
    
    //set pointer to player entity after deep copying 
    void setPlayer (void);

public:
    
    Scene_Play(GameEngine * gameEngine); //, const std::string & levelPath);

};