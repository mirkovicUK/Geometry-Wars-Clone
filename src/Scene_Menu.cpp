#include "Scene_Menu.h"
#include "Scene_Play.h"
#include "Common.h"
#include "Assets.h"
#include "GameEngine.h"
#include "Action.h"

Scene_Menu::Scene_Menu(GameEngine * gameEngine)
    :Scene(gameEngine) { init(); }

void Scene_Menu::init()
{
    registerAction(sf::Keyboard::W,   "UP");
    registerAction(sf::Keyboard::S,   "DOWN");
    registerAction(sf::Keyboard::Return,   "PLAY");
    registerAction(sf::Keyboard::Escape,   "QUIT");
    registerAction(sf::Keyboard::Q,   "CONTINUE");

    m_menuString.push_back("LEVEL 1");
    m_menuString.push_back("LEVEL 2");
    m_menuString.push_back("LEVEL 3");

    //sets text
    m_font.loadFromFile(m_game->getAssets().getFontConfig().F);
    m_menuText.setFont(m_font);
    m_menuText.setCharacterSize(64);

}

void Scene_Menu::update() 
{
    m_entities.update();

    if (m_hasEnded) {m_game->quit(); }

    //std::cout<<"nothing in update function Screne_Menu"<<std::endl;
}

void Scene_Menu::sDoAction(const Action & action)
{
    std::cout<<"Scene_Menu::sDoAction - begening"<<std::endl;
    if (action.type() == "START" )
    {
        if (action.name() == "UP")
        {
            if   (m_menuIndex > 0) { m_menuIndex--; }  
            else { m_menuIndex = m_menuString.size() - 1; } //up on first will circle to last
        }

        else if (action.name() == "DOWN")  { m_menuIndex = (m_menuIndex + 1) % m_menuString.size(); }

        else if (action.name() == "PLAY")
        {
            m_game->changeScene("PLAY", std::make_shared<Scene_Play>(m_game));
        }

        else if (action.name() == "QUIT")   { onEnd(); }

        else if (action.name() == "CONTINUE")   { m_game->changeScene("PLAY", nullptr); }
    }
}

void Scene_Menu::onEnd()
{
    m_hasEnded = true;
}

void Scene_Menu::sRender()
{
    //std::cout<<"sRender() - b4 everithing"<<std::endl;
    m_game->window().clear(sf::Color(4, 244, 4));

    sf::Font font;
    assert(font.loadFromFile(m_game->getAssets().getFontConfig().F));

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color(0, 0, 0));

    //draw levels, selected level is red
    float counter = 20; 
    for (int i = 0, n = m_menuString.size(); i < n; i++)
    {
        text.setPosition(40, counter);
        text.setString(m_menuString[i]);
        
        if (i == m_menuIndex) {text.setFillColor(sf::Color(250, 0, 0)); } // red
        m_game->window().draw(text);

        counter += 60;
        text.setFillColor(sf::Color(0, 0, 0));
    };

    //draw horizontal line
    float ScreenWidth = (float)m_game->window().getSize().x;
    float ScreenHeight = (float)m_game->window().getSize().y;
    sf::Vertex line[] = { sf::Vertex(sf::Vector2f(3 * ScreenWidth / 4, 0)), 
                          sf::Vertex(sf::Vector2f(3 * ScreenWidth / 4, ScreenHeight))};
    m_game->window().draw(line, 2, sf::Lines);

    //draw rest of instructions on the right of screen
    text.setPosition((3 * ScreenWidth / 4) + 40 , 20);
    text.setString("W      ::   UP");
    m_game->window().draw(text);
    
    text.setPosition((3 * ScreenWidth / 4) + 40 , 80);
    text.setString("S       ::   DOWN");
    m_game->window().draw(text);

    text.setPosition((3 * ScreenWidth / 4) + 40 , 140);
    text.setString("Enter ::   NEW GAME");
    m_game->window().draw(text);

    text.setPosition((3 * ScreenWidth / 4) + 40 , 200);
    text.setString("Exit    ::   QUIT");
    m_game->window().draw(text);

    text.setPosition((3 * ScreenWidth / 4) + 40 , 260);
    text.setString("Q       ::   CONTINUE");
    m_game->window().draw(text);

    m_game->window().display(); 

    //std::cout<<"sRender() - after everithing"<<std::endl;
}