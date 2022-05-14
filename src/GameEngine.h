#pragma once

#include "Common.h"
#include "Assets.h"
#include "Scene.h"

#include <memory>



typedef std::map<std::string, std::shared_ptr<Scene>>SceneMap;

class GameEngine
{
protected:
    sf::RenderWindow    m_window;       
    Assets              m_assets;       
    std::string         m_CurrentScene  =  "none"; 
    SceneMap            m_sceneMap;
    bool                m_runing = true;
    //bool                replay   = false; 

    void init (const std::string & path);
    void update();
    void sUserInput();
    std::shared_ptr<Scene> currentScene();  


public:
    GameEngine(const std::string & string); 

    void changeScene(const std::string & sceneName, std::shared_ptr<Scene>scene);
    void quit();
    void run();
    
    bool isRuning();

    // getters
    const Assets & getAssets() const;
    sf::RenderWindow & window(); 
    std::string & currentSceneString() ;

};