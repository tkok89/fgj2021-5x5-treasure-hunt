#include "Resources.h"

#include <iostream>
#include <assert.h>

#ifdef TARGET_OS_MAC
#include "ResourcePath.hpp"
#else
std::string resourcePath(void){
    return "";
}
#endif

Resources::Resources()
{
    bool success = false;
    success = tileTextures[0].loadFromFile(resourcePath() + "assets/block.png");
    assert(success);
    
    success = tileTextures[1].loadFromFile(resourcePath() + "assets/pawn.png");
    assert(success);
    
    if (!font.loadFromFile(resourcePath() + "assets/Vera.ttf"))
    {
        std::cerr << "Error loading Vera.ttf" << std::endl;
    }
}
