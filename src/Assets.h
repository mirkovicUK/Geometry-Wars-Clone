#pragma once

#include "Common.h"


struct PlayerConfign {int SR, CR, FR, FG, FB, OR, OG, OB, OT, V; float S; };
struct EnemyConfig   {int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI; float SMIN, SMAX; };
struct BulletConfig  {int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L; float S; };
struct FontConfig    {int S, R, G, B; std::string F; };
struct WindowConfig  {int W, H, FL, FS; }; 
struct SpecialWeponConfig  {int reloadTime; };

class Assets
{
    PlayerConfign       m_playerConfig;
    EnemyConfig         m_enemyConfig;
    BulletConfig        m_bulletConfig;
    FontConfig          m_fontConfig;
    WindowConfig        m_windowConfig;
    SpecialWeponConfig  m_swConfig;

public:
    Assets();
    Assets(const std::string & config);

    void loadFromFile(const std::string & path);
    void print()const;

    // geters
   const WindowConfig  & getWindowConfig() const;
   const FontConfig    & getFontConfig()   const;
   const PlayerConfign & getPlayerConfig() const;
   const BulletConfig  & getBulletConfig() const;
   const EnemyConfig   & getEnemyConfig () const;
   const SpecialWeponConfig & getSwConfig() const;
};