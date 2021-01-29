#include "Resources.h"
#include <iostream>

#ifdef TARGET_OS_MAC
#include "ResourcePath.hpp"
#else
std::string resourcePath(void){
    return "";
}
#endif

Resources::Resources()
{
	tileTextures[0].loadFromFile(resourcePath() + "assets/block.png");
    
	tileTextures[1].loadFromFile(resourcePath() + "assets/pawn.png");
    
    if (!font.loadFromFile(resourcePath() + "assets/Vera.ttf"))
    {
        std::cerr << "Error loading Vera.ttf" << std::endl;
    }
}
