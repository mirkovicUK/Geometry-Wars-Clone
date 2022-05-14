#include "Common.h"
#include "Scene.h"


class Scene_Menu : public Scene
{
    std::vector<std::string>        m_menuString;
    sf::Text                        m_menuText;
    sf::Font                        m_font;
    int                             m_menuIndex = 0;

public:

    Scene_Menu(GameEngine * gameEngine);
    void init();
    
    void update() override;
    void sRender() override;
    void sDoAction(const Action & action) override;
    void onEnd() override;
};