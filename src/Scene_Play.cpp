#include "Scene_Play.h"
#include "Common.h"
#include "Assets.h"
#include "GameEngine.h"
#include "Action.h"

Scene_Play::Scene_Play(GameEngine * gameEngine)
:Scene(gameEngine)  { init(); }


void Scene_Play::init()
{
    std::cout<< "Scene_Play::init()"<<std::endl;
    registerAction(sf::Keyboard::W,       "UP");
    registerAction(sf::Keyboard::S,       "DOWN");
    registerAction(sf::Keyboard::A,       "LEFT");
    registerAction(sf::Keyboard::D,       "RIGHT");
    registerAction(sf::Mouse::Left +10,   "FIRE");      //same as letter A, add 10 to it makes K(will not be in used at this game)
    registerAction(sf::Mouse::Right +10,  "SW");        //same as letter B, add 10 to it make L (will not be in used at this game)
    registerAction(sf::Keyboard::Escape,  "QUIT");
    registerAction(sf::Keyboard::Space,   "PAUSE");
    registerAction(sf::Keyboard::R,   "REPLAY");
    registerAction(sf::Keyboard::P,   "PlayREPLAY");

    spawnPlayer();
    spawnEnemy();
    srand ((time(NULL)));
    setText();
    setBackground();
}

 void Scene_Play::update()
 {
   
    if (m_paused)
    {
        m_entities.update();

        if (m_hasEnded) {m_game->changeScene("MENU", nullptr), m_hasEnded = false; }
        m_currentFrame++;
    }
    else
    {   
        
        m_entities.update();

        sMovement();

        SpecialWeaponControl(m_player);
        
        sEnemySpawner();
      
        sLifespan();
     
        sCollision();

        m_currentFrame++;
      
        if (m_hasEnded) {m_game->changeScene("MENU", nullptr), m_hasEnded = false; }
       
    }
 }
    

 void Scene_Play::onEnd()
 {
    m_hasEnded = true;
 }

 void Scene_Play::sRender() 
 {
    
    m_game->window().clear(); 
    m_game->window().draw(m_sprite);

    for (auto e : m_entities.getEntities()) 
    {
        if (e->cTransform && e->cShape)
        {
            // set the position of the shape 
            e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);

            // set the rotation of the shape 
            e->cTransform->angle += 1.0f;
            e->cShape->circle.setRotation(e->cTransform->angle);

            m_game->window().draw(e->cShape->circle);
        }
    }

    
    m_game->window().draw(m_text);
    m_game->window().draw(m_life);
    m_game->window().draw(m_specialWepon);
    
    // Display
    m_game->window().display();    
}

 void Scene_Play::sDoAction(const Action & action)
 {
    if (action.type() == "START")
    {
        if      (action.name() == "UP")    { m_player->cInput->up   = true; }
        else if (action.name() == "DOWN")  { m_player->cInput->down = true; }
        else if (action.name() == "LEFT")  { m_player->cInput->left = true; }
        else if (action.name() == "RIGHT") { m_player->cInput->right = true;}
        else if (action.name() == "QUIT")  { onEnd(); }       
        else if (action.name() == "SW")  
        { 
            if (!m_paused && !m_player->cSpecialWeapon->reload) 
            { 
                m_player->cSpecialWeapon->active = true; //activate SpecialWeapon
                m_player->cSpecialWeapon->countdown = m_currentFrame;
            }
        }
        else if (action.name() == "FIRE")//need Action2
        {            
            if (!m_paused)// if game is not paused spawn bullet
            {
                std::cout<<"LEFT MOUSE from Scene_Play::sDoAcion" << std::endl;
                try
                {
                  const Action2 & action2 = dynamic_cast<const Action2 &>(action);        
                  spawnBullet(m_player, Vec2(action2.getPositionX(), action2.getPositionY()));
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                }                
            }
        }
        else if (action.name() == "PAUSE")  
        {  
            m_paused ? m_paused = false : m_paused = true;
        }
        //else if (action.name() == "REPLAY") {m_game->StartReplay(); RecordReplay(); setReplayCurentFrame(); }
    }
    else if (action.type() == "END")
    {
        if      (action.name() == "UP"  )  {m_player->cInput->up    = false; }
        else if (action.name() == "DOWN")  {m_player->cInput->down  = false; }
        else if (action.name() == "LEFT")  {m_player->cInput->left  = false; }
        else if (action.name() == "RIGHT") {m_player->cInput->right = false; }
        //else if (action.name() == "PlayREPLAY") { m_game->StopReplay(); PlayReplay(); setReplay(); }
    }
 }

 void Scene_Play::spawnPlayer() 
{ 
    auto entity = m_entities.addEntity("player");

    //player spawninig position
    sf::Vector2u windowSize = m_game->window().getSize();
    Vec2 playerSpawnPos;
    playerSpawnPos.x = static_cast<float>(windowSize.x / 2);
    playerSpawnPos.y = static_cast<float>(windowSize.y / 2);
    
    //sMovement will create velocity vec for player basee on sInput
    Vec2 playerVelocity = {0,0}; 
    
    //Transform so it spawns at center of screan with config speed and angle 0
    entity ->cTransform = std::make_shared<CTransform>(playerSpawnPos, playerVelocity, 0.0f);

    // the entity's shape will have as per config file
    entity->cShape = std::make_shared<CShape>(m_game->getAssets().getPlayerConfig().SR, 
    m_game->getAssets().getPlayerConfig().V, 
    sf::Color(m_game->getAssets().getPlayerConfig().FR,
              m_game->getAssets().getPlayerConfig().FG, 
              m_game->getAssets().getPlayerConfig().FB), 
    sf::Color(m_game->getAssets().getPlayerConfig().OR,m_game->getAssets().getPlayerConfig().OG, 
    m_game->getAssets().getPlayerConfig().OB), m_game->getAssets().getPlayerConfig().OT);

    // imput component   
    entity->cInput = std::make_shared<CInput>();

    // collision component 
    entity->cCollision = std::make_shared<CCollision>(m_game->getAssets().getPlayerConfig().CR);

    //CSpecialWeapon component 
    entity->cSpecialWeapon = std::make_shared<CSpecialWeapon>(m_game->getAssets().getSwConfig().reloadTime);

    // shortcut To player 
    m_player = entity;
}


// spawn enemy on random position
void Scene_Play::spawnEnemy()
{
    auto entity = m_entities.addEntity("enemy");

    // get sf::window size
    sf::Vector2u windowSize = m_game->window().getSize();

    //randomise enemySpawnPos every time spawnEnemy() is called, its spawned inside screen
    Vec2 enemySpawnPos;
    enemySpawnPos.x = static_cast<float>( //x coordinate range [0+radious, w - radious]
        randomiser((0 + m_game->getAssets().getEnemyConfig().SR), 
        (windowSize.x - m_game->getAssets().getEnemyConfig().SR)));

    enemySpawnPos.y = static_cast<float>(//y cordinate range [0 + r, h - r]
        randomiser((0 + m_game->getAssets().getEnemyConfig().SR), 
        (windowSize.y - m_game->getAssets().getEnemyConfig().SR)));

    //2xPlayerRadious is no spawning area
    float noSpawningArea = m_player->cShape->circle.getRadius() * 4;
    if (m_player->cTransform->pos.dist(enemySpawnPos) < noSpawningArea)
    {
        //edge cases are not good might spawn outside of screen now !!!!!
        enemySpawnPos.x += noSpawningArea;
        enemySpawnPos.y += noSpawningArea;
    }
    
    //initialise velocity based on rand angle [0-6.28r] and rand speed[SMIN-SMAX(config)]
    Vec2 velocity;
    double randAngle = randomiser(0, 6.28); 
    float  randSpeed = randomiser(m_game->getAssets().getEnemyConfig().SMIN,
                                  m_game->getAssets().getEnemyConfig().SMAX);
    velocity = {(float)(randSpeed*cos(randAngle)), (float)(randSpeed*sin(randAngle))};
    
    // CTransform 
    entity->cTransform = std::make_shared<CTransform>(enemySpawnPos, velocity, 0.0f);

    // randomise shape vertices
    int vertices = randomiser(m_game->getAssets().getEnemyConfig().VMIN, 
                              m_game->getAssets().getEnemyConfig().VMAX);
    // randomise enemy color
    int randR, randG, randB;
    randR = randomiser(0, 255); randG = randomiser(0, 255); randB = randomiser(0, 255);

    // CShape 
    entity->cShape = std::make_shared<CShape>(m_game->getAssets().getEnemyConfig().SR, vertices, 
                     sf::Color(randR, randB, randG), sf::Color(m_game->getAssets().getEnemyConfig().OR, 
                     m_game->getAssets().getEnemyConfig().OG, m_game->getAssets().getEnemyConfig().OB),
                     m_game->getAssets().getEnemyConfig().OT);

    // add colision component 
    entity->cCollision = std::make_shared<CCollision>(m_game->getAssets().getEnemyConfig().CR);

    // CScore
    entity->cScore = std::make_shared<CScore>(100 * vertices);

    // record when the most recent enemy was spawnew
    m_lastEnemySpawnTime = m_currentFrame;
}

int Scene_Play::randomiser (int min, int max)
{
    return (min + (rand() % ( max + 1 - min)));
}

float Scene_Play::randomiser (float min, float max)
{
    float random = static_cast<float>(rand() / static_cast<float>(RAND_MAX));
    float diff = max - min;
    
    return min + random * diff;
}

void Scene_Play::spawnSmallEnemies(std::shared_ptr<Entity> e)
{
    
    // get number of verices from enemy entity
    std::size_t vertices = e->cShape->circle.getPointCount();
    double rotation = 6.28 / vertices; 
    double angle = 0;
    
    for (int i = 0; i < vertices; i++) 
    {
        auto smallEnemies = m_entities.addEntity("smallEnemies"); 
    
        // Vec2 velocity (Speed * cos(angle), Speed * sin(angle))
        Vec2 velocity = {static_cast<float>(0.9f * cos(angle)), static_cast<float>(0.9f * sin(angle)) };

        smallEnemies->cTransform = std::make_shared<CTransform>(e->cTransform->pos, 
                                                                velocity, 0.0f);
        angle += rotation; //update 

        float radious = e->cShape->circle.getRadius() / 2; 
        auto fillColor = e->cShape->circle.getFillColor();
        auto outlineColor = e->cShape->circle.getOutlineColor();
        auto thickness = e->cShape->circle.getOutlineThickness();

        smallEnemies->cShape     = std::make_shared<CShape>(radious, vertices, fillColor,
                                   outlineColor,thickness);
        smallEnemies->cLifespan  = std::make_shared<CLifespan>(m_game->getAssets().getEnemyConfig().L);
        smallEnemies->cCollision = std::make_shared<CCollision>(e->cCollision->radious / 2);
        smallEnemies->cScore     = std::make_shared<CScore>(3 * e->cScore->score);
    }

}

void Scene_Play::setText()
{
    if(!m_font.loadFromFile(m_game->getAssets().getFontConfig().F)) 
    {std::cout<<"Unable to load font\n"; }//load font

    //set score text
    m_text.setFont(m_font); 
    m_text.setCharacterSize(m_game->getAssets().getFontConfig().S); 
    m_text.setFillColor(sf::Color(m_game->getAssets().getFontConfig().R,
                        m_game->getAssets().getFontConfig().G, m_game->getAssets().getFontConfig().B));
    m_text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    m_text.setPosition(0, 0);
    m_text.setString("Score:0");

    //Set life text
    m_life.setFont(m_font);
    m_life.setCharacterSize(m_game->getAssets().getFontConfig().S);
    m_life.setFillColor(sf::Color(m_game->getAssets().getFontConfig().R, 
                         m_game->getAssets().getFontConfig().G, m_game->getAssets().getFontConfig().B));
    m_life.setStyle(sf::Text::Bold | sf::Text::Underlined);
    sf::Vector2u windowSize = m_game->window().getSize();
    m_life.setPosition(windowSize.x / 2, 0);

    //int to string 
    std::stringstream ss;
    ss << m_lifeCounter;
    std::string lifeCounter = ss.str();
    m_life.setString("LIFE: " + lifeCounter);

    m_specialWepon.setFont(m_font);
    m_specialWepon.setCharacterSize(m_game->getAssets().getFontConfig().S);
    if(m_player->cSpecialWeapon->reloadTime == 120)
    {
        m_specialWepon.setFillColor(sf::Color(0, 0, 255));
    }
    else
    {
        m_specialWepon.setFillColor(sf::Color(255, 0, 0));
    }

    m_specialWepon.setStyle(sf::Text::Bold | sf::Text::Underlined);
    sf::FloatRect position = m_specialWepon.getLocalBounds();

    m_specialWepon.setPosition(windowSize.x / 2 + 250, 0); //HARD CODED POSITIN
    m_specialWepon.setString("Bullet Storm");

    

}

//calculate score 
void Scene_Play::calcScore(const std::shared_ptr<Entity> entity)
{
    m_score += entity->cScore->score;

    // set score sa text
    std::stringstream ss;
    ss << m_score;
    std::string score = ss.str();
    m_text.setString("Score:" + score);
}

// calculate life
void Scene_Play::calcLife (void)
{
    if (m_lifeCounter == 0) { /*m_running = false;*/ }

    std::stringstream ss;
    ss << m_lifeCounter;
    std::string lifeCounter = ss.str();
    m_life.setString("LIFE:" + lifeCounter);
}


void Scene_Play::setBackground()
{
    sf::Vector2u size = m_game->window().getSize();
    
    m_texture.loadFromFile("picture.jpeg", sf::IntRect(0, 0, size.x, size.y));
    
    
    m_sprite.setTexture(m_texture);
    m_sprite.setOrigin(0,0);
}


// spawns a bullet from a given entity to a target locatio
void Scene_Play::spawnBullet(std::shared_ptr<Entity> entity, const Vec2 & target) 
{
    // create bullet entity
    auto bullet = m_entities.addEntity("bullet");
    // calculatin angle for cTransform
    float angle = atan2f((target.y - entity->cTransform->pos.y), (target.x - entity->cTransform->pos.x));

    // initialise components 
    //velocity Vec2 (speed*cos(angle), speed*sin(angle))
    bullet->cTransform = std::make_shared<CTransform>(Vec2(entity->cTransform->pos.x,
                     entity->cTransform->pos.y), Vec2(m_game->getAssets().getBulletConfig().S * (float)cos(angle),
                      m_game->getAssets().getBulletConfig().S * (float)sin(angle)), angle);

    bullet->cShape = std::make_shared<CShape>(m_game->getAssets().getBulletConfig().SR, 
                                              m_game->getAssets().getBulletConfig().V, 
               sf::Color(m_game->getAssets().getBulletConfig().FR, 
                         m_game->getAssets().getBulletConfig().FG,
                         m_game->getAssets().getBulletConfig().FB), 
               sf::Color(m_game->getAssets().getBulletConfig().OR, 
               m_game->getAssets().getBulletConfig().OG, 
               m_game->getAssets().getBulletConfig().OB), 
               m_game->getAssets().getBulletConfig().OT);

    bullet->cLifespan = std::make_shared<CLifespan>(m_game->getAssets().getBulletConfig().L);

    bullet->cCollision = std::make_shared<CCollision>(m_game->getAssets().getBulletConfig().SR);
}


// right mouse change flag to true, Weponcontrol fire 3 times every half of sec
void Scene_Play::SpecialWeaponControl(std::shared_ptr<Entity> entity)
{
    if (entity->cSpecialWeapon->active && (entity->cSpecialWeapon->count < 8) && 
       (entity->cSpecialWeapon->countdown == m_currentFrame) )
    {
        spawnSpecialWeapon(entity); // call to special wepon
        entity->cSpecialWeapon->count++; //it fire 8 times
        entity->cSpecialWeapon->countdown += 15; //fire every  1/4 sec
    } 

    //reset active flag and count for new round sets cooling of flag on
    if (entity->cSpecialWeapon->count == 8) 
    {
        entity->cSpecialWeapon->active = false; 
        entity->cSpecialWeapon->count = 0; 
        entity->cSpecialWeapon->reload = true;
    }

    if (entity->cSpecialWeapon->reload)
    {
        if (entity->cSpecialWeapon->reloadTime == 0) 
        {
            entity->cSpecialWeapon->reload = false;  // reloaded
            entity->cSpecialWeapon->reloadTime =m_game->getAssets().getSwConfig().reloadTime +1; // -- in next step
        }
        entity->cSpecialWeapon->reloadTime --;
        
    }

    // sets color of text object drawn on screan
    if(m_player->cSpecialWeapon->reloadTime == m_game->getAssets().getSwConfig().reloadTime)
    {
        m_specialWepon.setFillColor(sf::Color(0, 0, 255));
    }
    else
    {
        m_specialWepon.setFillColor(sf::Color(255, 0, 0));
    }
}
void Scene_Play::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
   // shoots all around (360) 
   double rotation = 0.1; 
   double angle = 0;
   while (angle < 6.28)
   {
       //Vec2 position is relativ to player entity
       Vec2 playerPos = {entity->cTransform->pos};
       Vec2 position = {(float)(cos(angle) + playerPos.x), (float)(sin(angle) + playerPos.y)};

       spawnBullet(entity,position);
       angle += rotation;
    }
}

void Scene_Play::sMovement()
{
    Vec2 playerVelocity(0,0); //takes in values based on imput(lefr&right) and speed from config

    if (m_player->cInput->left)
    {
        //if player not inWindow on X asix Velocity.x = 0;
        if (!m_player->cInput->inWinXleft) 
        {
            playerVelocity.x = 0;  // x speed = 0
            m_player->cInput->inWinXleft = true;
        }
        else//player is in window
        {
            playerVelocity.x -= m_game->getAssets().getPlayerConfig().S; 
            //std::cout<<"Velocity.X = " <<playerVelocity.x<<std::endl;
        }
    }
    
    if (m_player->cInput->right)
    {
        std::cout<<"sMovement() 1st"<<std::endl;
        if (!m_player->cInput->inWinXright) //set by collision system
        {
            playerVelocity.x = 0; 
            m_player->cInput->inWinXright = true;
            std::cout<<"sMovement 2nd"<<std::endl;
        }
        else
        {
            playerVelocity.x += m_game->getAssets().getPlayerConfig().S;
            std::cout<<"sMovement() : Velocity.X = " <<playerVelocity.x<<std::endl;
        }
    }

    if (m_player->cInput->up)
    {
        if (!m_player->cInput->inWinYup) 
        {
            playerVelocity.y = 0; 
            m_player->cInput->inWinYup = true;
        }
        else
        {
            playerVelocity.y -= m_game->getAssets().getPlayerConfig().S;
            //std::cout<<"Velocity.Y = " <<playerVelocity.y<<std::endl;
        }
    }

    if (m_player->cInput->down)
    {
        if (!m_player->cInput->inWinYdown) 
        {
            playerVelocity.y = 0; 
            m_player->cInput->inWinYdown = true;
        }
        else
        {
            playerVelocity.y += m_game->getAssets().getPlayerConfig().S;
            //std::cout<<"Velocity.Y = " <<playerVelocity.y<<std::endl;
        }
    }
    
    //make sure that diagonal(eg. left+up) speed stays same as config speed
    if (playerVelocity.x * playerVelocity.x  == playerVelocity.y * playerVelocity.y //compare valuse regardless of signe
          && playerVelocity.x != 0 && playerVelocity.y != 0)
    {
        if (playerVelocity.x < 0) //Check direction
        {
            playerVelocity.x = m_game->getAssets().getPlayerConfig().S /1.4142;
            playerVelocity.x *= -1; //keep direction of velocity vector
            //std::cout<<"!!Velocity.X = " <<playerVelocity.x<<std::endl;
        }
        else
        {
            playerVelocity.x = m_game->getAssets().getPlayerConfig().S /1.4142;
            //std::cout<<"!!Velocity.X = " <<playerVelocity.x<<std::endl;
        }
        
       if (playerVelocity.y < 0)
       {
           playerVelocity.y = m_game->getAssets().getPlayerConfig().S /1.4142;
           playerVelocity.y *= -1;
           //std::cout<<"!!Velocity.Y = " <<playerVelocity.y<<std::endl;
       }
       else
       {
            playerVelocity.y = m_game->getAssets().getPlayerConfig().S /1.4142;
           // std::cout<<"!!Velocity.Y = " <<playerVelocity.y<<std::endl;
       }
        
    }

    // sets player internal velocity as collecting variable
     m_player->cTransform->velocity = playerVelocity; 
    

    // movement speed update all entities
    for (auto & e : m_entities.getEntities()) 
    {
        if(e->cTransform && e->cShape)
        {
            e->cTransform->pos += e->cTransform->velocity;
        }
    }

}

void Scene_Play::sCollision()
{
    std::cout<<"sCollision Start"<<std::endl;

    // player enemy collision 
    for (auto p : m_entities.getEntities("player"))
    {

        for (auto e : m_entities.getEntities("enemy")) 
        {
            float distanceSquare = ((e->cTransform->pos.x)- (p->cTransform->pos.x)) *  // (x2 - x1) * (x2 - x1)
                                   ((e->cTransform->pos.x) - (p->cTransform->pos.x)) + 
                                   ((e->cTransform->pos.y)- (p->cTransform->pos.y)) * //  (y2 - y1) * (y2 - y1)
                                   ((e->cTransform->pos.y)- (p->cTransform->pos.y));  

            float collisionRadiousSquare = (float)((p->cCollision->radious + e->cCollision->radious) 
                                                * (p->cCollision->radious + e->cCollision->radious));

            // check for colision avoid using sqrtf
            if (distanceSquare <= collisionRadiousSquare)
            {
                std::cout<<"sCollision IS IT IN???"<<std::endl;
                calcScore(e);

                m_lifeCounter--;
                calcLife();

                p->destroy();
                e->destroy();

                spawnPlayer();
                spawnSmallEnemies(e);
            }
        }
    }

    // enemy bullet collision 
    for (auto e : m_entities.getEntities("enemy"))
    {
        for (auto b : m_entities.getEntities("bullet"))
        {
            // caluculate collision avoid using sqrtf
            float distanceSquare = ((e->cTransform->pos.x)- (b->cTransform->pos.x)) *  // (x2 - x1) * (x2 - x1)
                                   ((e->cTransform->pos.x)- (b->cTransform->pos.x)) + 
                                   ((e->cTransform->pos.y)- (b->cTransform->pos.y)) * //  (y2 - y1) * (y2 - y1)
                                   ((e->cTransform->pos.y)- (b->cTransform->pos.y));

            float collisionRadiousSquare = (float)((b->cCollision->radious + e->cCollision->radious) 
                                          * (b->cCollision->radious + e->cCollision->radious));
            
            // check for colision avoid using sqrtf
            if (distanceSquare <= collisionRadiousSquare)
            {
                calcScore(e);

                b->destroy();
                e->destroy();

                spawnSmallEnemies(e);
            }
        }
    }
     std::cout<<"sCollision after enemy bullet colision"<<std::endl;

    // enemy window collision 
    for (auto e: m_entities.getEntities("enemy"))
    {
        // get sf::window size
        sf::Vector2u windowSize = m_game->window().getSize();
        float radious = e->cShape->circle.getRadius();

        if (e->cTransform->pos.x - radious <= 0)   {e->cTransform->velocity.x *= -1; }
        if (e->cTransform->pos.x + radious >= windowSize.x) 
        {
            e->cTransform->velocity.x *= -1; 
        }

        if (e->cTransform->pos.y - radious <= 0) {e->cTransform->velocity.y *= -1; }
        if (e->cTransform->pos.y + radious >= windowSize.y) 
        {
            e->cTransform->velocity.y *= -1; 
        }
    }
    std::cout<<"sCollision after enemy window colision"<<std::endl;

    for (auto p : m_entities.getEntities("player"))
    {
        // get sf::window size
        sf::Vector2u windowSize = m_game->window().getSize();
        float radious = p->cShape->circle.getRadius();

        // if player is out of window on all 4 side side Flag it false 
        int playerThickness = m_player->cShape->circle.getOutlineThickness();
        if (p->cTransform->pos.x - radious - playerThickness <= 0) 
        {
            p->cInput->inWinXleft = false;
        }

        if (p->cTransform->pos.x + radious +playerThickness >= windowSize.x)
        {
            p->cInput->inWinXright = false;
        }

        if (p->cTransform->pos.y - radious -playerThickness <= 0) 
        {
            p->cInput->inWinYup = false; 
        }

        if (p->cTransform->pos.y + radious + playerThickness >= windowSize.y)
        {
            p->cInput->inWinYdown = false; 
        }
    }

    // smallEnemies Player colision
    for (auto se : m_entities.getEntities("smallEnemies"))
    {
        float distance = m_player->cTransform->pos.dist(se->cTransform->pos);
        float colisionRadious = se->cCollision->radious + m_player->cCollision->radious;
        if (distance <= colisionRadious)
        {
            m_lifeCounter--;
            calcLife();

            calcScore(se);

            se->destroy();
            m_player->destroy();

            spawnPlayer();
        }
    }

    // smallEnemies bullets collision
    for (auto se : m_entities.getEntities("smallEnemies"))
    {
        for (auto b : m_entities.getEntities("bullet"))
        {
            float diastance = se->cTransform->pos.dist(b->cTransform->pos);
            float colisionRadious = se->cCollision->radious + b->cCollision->radious;
            if (diastance <= colisionRadious)
            {
                calcScore(se); 
                
                se->destroy();
                b->destroy();
            }
        }
    }

    // small enemy window collision to stay inside window
    for (auto se : m_entities.getEntities("smallEnemies"))
    {
        sf::Vector2u windowSize = m_game->window().getSize();
        float radious = se->cShape->circle.getRadius();

        if (se->cTransform->pos.x - radious <= 0) {se->cTransform->velocity.x *= -1; }
        if (se->cTransform->pos.x + radious >= windowSize.x) {se->cTransform->velocity.x *= -1;}

        if (se->cTransform->pos.y - radious <= 0) {se->cTransform->velocity.y *= -1; }
        if (se->cTransform->pos.y + radious >= windowSize.y) {se->cTransform->velocity.y *= -1;}
    }
}

void Scene_Play::sEnemySpawner()
{
    int howLong = m_currentFrame - m_lastEnemySpawnTime;

    if (m_game->getAssets().getEnemyConfig().SI <= howLong) { spawnEnemy(); }
}

void Scene_Play::sLifespan()
{
    
    
    for (auto e : m_entities.getEntities())
    {
        if (!e->cLifespan) {continue; } //does entity have cLifespan
        if (e->cLifespan->remaining > 0) {e->cLifespan->remaining -= 1; } //-1 per frame
        if (e->isActive())
        {
            ////sets color of entity
            auto fillColor = e->cShape->circle.getFillColor();
            auto outlineColor = e->cShape->circle.getOutlineColor();

            // calculate new alpha channel and set new each frame NOT GOOD FORMULA
            // need to come back to it !!!!
            int NewAlphaOutline = outlineColor.a - static_cast<int>((outlineColor.a / e->cLifespan->total) + 0.5f);
            int newAlpha = fillColor.a - static_cast<int>((fillColor.a / e->cLifespan->total) + 0.5f);
            
            sf::Color newColor(fillColor.r, fillColor.g, fillColor.b, newAlpha);
            sf::Color newOutlineColor(outlineColor.r, outlineColor.g, outlineColor.b, NewAlphaOutline);
           
            e->cShape->circle.setFillColor(newColor);
            e->cShape->circle.setOutlineColor(newOutlineColor);
        }
        if (e->cLifespan->remaining == 0) {e->destroy(); }
    }
}

void Scene_Play::setPlayer (void)
{
    for (auto p : m_entities.getEntities())
    {
        if (p->tag() == "player")
        {
            m_player = p;
        }
    }
}