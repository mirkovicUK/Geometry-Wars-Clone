#include "GameEngine.h"
#include "Scene_Play.h"
#include "Scene_Menu.h"

GameEngine::GameEngine (const std::string & path) { init(path); }


void GameEngine::init (const std::string & path) //read config and assets
{
    m_assets.loadFromFile(path); //load assets/config

    auto WindowConfig = m_assets.getWindowConfig().FS; //(FS:int)
    std::vector<sf::VideoMode> Resolution = sf::VideoMode::getFullscreenModes(); // system video modes

    if (WindowConfig) //full screen
    {
        //create full screen with best sistem Resolution[0]
        m_window.create(sf::VideoMode(Resolution[0].width, Resolution[0].height, 
                             Resolution[0].bitsPerPixel), "GAME", sf::Style::Fullscreen);
        m_window.setFramerateLimit(m_assets.getWindowConfig().FL); //set frame limit 60fps
    }
    else
    {
        m_window.create(sf::VideoMode(m_assets.getWindowConfig().W, m_assets.getWindowConfig().H), "GAME");
        m_window.setFramerateLimit(m_assets.getWindowConfig().FL);
    }

    
    changeScene("MENU", std::make_shared<Scene_Menu>(this)); 
    std::cout<<"First scene made!"<<std::endl;
    
}

std::shared_ptr<Scene> GameEngine::currentScene()
{
    //std::cout<<"GameEngine::currentScene() , m_CurrentScene is:" <<m_CurrentScene<<std::endl;
    return m_sceneMap[m_CurrentScene];
}

bool GameEngine::isRuning()
{
    return m_runing && m_window.isOpen();
}

sf::RenderWindow & GameEngine::window() 
{
    return m_window;
}

void GameEngine::sUserInput()
{
   // std::cout<<"sUserInput() - first line"<<std::endl;
    sf::Event event;

    while(m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            quit(); 
        }

        if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
        {
            std::cout <<"event key code:"<<event.key.code<<std::endl;
            //letters K & L will not be considered as valid input in this game
            if (event.key.code == 10 || event.key.code == 11) { continue; } 
            if(currentScene()->getActionMap().find(event.key.code) == currentScene()->getActionMap().end())
            {continue; }
  
           const std::string actionType = (event.type == sf::Event::KeyPressed) ? "START" : "END";
           const std::string actionName = currentScene()->getActionMap().at(event.key.code);

          Action action(actionName, actionType);//create action obj
          currentScene()->doAction(action);    //ship it to doAction;
        }

        if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased)
        {
            // in Scene_Play::init() , registrAction() 10 is added to mouse click (left and right)
            int eventKeyCode = event.key.code + 10;
            if (currentScene()->getActionMap().find(eventKeyCode) == currentScene()->getActionMap().end())
            { continue; }

            const std::string actionType = (event.type == sf::Event::MouseButtonPressed) ? "START" : "END";
            const std::string ActionName = currentScene()->getActionMap().at(eventKeyCode);

            if (event.mouseButton.button == sf::Mouse::Left) //Action2 Object, we need click position
            {
                const int positionX = event.mouseButton.x;
                const int positionY = event.mouseButton.y;
                Action2 action2(positionX, positionY, ActionName, actionType);//create Action2 obj
                currentScene()->doAction(action2); 
            }
            else
            {
                Action action(ActionName, actionType);
                currentScene()->doAction(action);
            }
            
        }
    }  
}


const Assets & GameEngine::getAssets() const
{
    return m_assets;
}

void GameEngine::run() // main game loop
{
    while (isRuning())
    {
        update();
    }
}

void GameEngine::update()
{
    sUserInput();
    currentScene()->update();
    currentScene()->sRender();
}

void GameEngine::quit()
{
    m_runing = false;
}

void GameEngine::changeScene(const std::string & sceneName, std::shared_ptr<Scene>scene_ptr)
{
    if (scene_ptr) // is active pointer
    {
        m_sceneMap[sceneName] = scene_ptr; //assigne or repleace 
        m_CurrentScene = sceneName;       //set currentScene
    }
    else //not active(nullptr) 
    {
        if (m_sceneMap.find(sceneName) == m_sceneMap.end()) //nullptr and key is not in map
        {
            std::cout<<"FAIL to changeScene() insuficient information  GameEngine.cpp "<<std::endl;
        }
        else
        {
            // nullptr received but there is {key,value} in map load it to curent scene
            m_CurrentScene = sceneName; 
        }
    }
}

std::string & GameEngine::currentSceneString()
{
    return m_CurrentScene;
}


