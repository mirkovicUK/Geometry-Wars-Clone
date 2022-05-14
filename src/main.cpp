#include "GameEngine.h"
#include "Common.h"


int main()
{
    GameEngine g("config.txt");
    std::cout<<"main b4 g.run()"<<std::endl;
    g.run();

}