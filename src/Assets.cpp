#include "Assets.h"

Assets::Assets() {}
Assets::Assets(const std::string & config) { loadFromFile(config); }

void Assets::loadFromFile(const std::string & path)
{
    std::string string;
    std::ifstream fin(path);

    while(fin >> string)
    {
        if (string == "Player")
        {
            fin >> m_playerConfig.SR >> m_playerConfig.CR >> m_playerConfig.S 
                >> m_playerConfig.FR >> m_playerConfig.FG >> m_playerConfig.FB
                >> m_playerConfig.OR >> m_playerConfig.OG >> m_playerConfig.OB
                >> m_playerConfig.OT >> m_playerConfig.V;
                continue;
        }
        else if (string == "Enemy")
        {
            fin >> m_enemyConfig.SR >> m_enemyConfig.CR >> m_enemyConfig.SMIN
                >> m_enemyConfig.SMAX >> m_enemyConfig.OR >> m_enemyConfig.OG
                >> m_enemyConfig.OB >> m_enemyConfig.OT >> m_enemyConfig.VMIN
                >> m_enemyConfig.VMAX >> m_enemyConfig.L >> m_enemyConfig.SI;
        }
        else if (string == "Bullet")
        {
            fin >> m_bulletConfig.SR >> m_bulletConfig.CR >> m_bulletConfig.S 
                >> m_bulletConfig.FR >> m_bulletConfig.FG >> m_bulletConfig.FB
                >> m_bulletConfig.OR >> m_bulletConfig.OG >> m_bulletConfig.OB
                >> m_bulletConfig.OT >> m_bulletConfig.V  >> m_bulletConfig.L;
        }
        else if (string == "Font")
        {
            fin >> m_fontConfig.F >> m_fontConfig.S >> m_fontConfig.R >> m_fontConfig.G 
                >> m_fontConfig.B;
        }
        else if (string == "Window")
        {
            fin >> m_windowConfig.W >> m_windowConfig.H >> m_windowConfig.FL >> m_windowConfig.FS;
        }
        else if (string == "SpecialWeapon")
        {
            fin >> m_swConfig.reloadTime;
        }
    }
}

const WindowConfig & Assets::getWindowConfig() const
{
    return m_windowConfig;
}

const FontConfig & Assets::getFontConfig() const
{
    return m_fontConfig;
}

void Assets::print () const
{
    std::cout<< m_swConfig.reloadTime << std::endl;
}

const PlayerConfign & Assets::getPlayerConfig() const
{
    return m_playerConfig;
}

 const BulletConfig  & Assets::getBulletConfig() const
 {
     return m_bulletConfig;
 }

const EnemyConfig   & Assets::getEnemyConfig () const
{
    return m_enemyConfig;
}

const SpecialWeponConfig & Assets::getSwConfig() const
{
    return m_swConfig;
}