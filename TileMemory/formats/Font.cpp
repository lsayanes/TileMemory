
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>

#include "Font.h"

namespace format
{

    Font::Font(const std::string &path, float szf) :
        pData{ nullptr }, size{ szf }, r{ 0 }, g{ 0 }, b{ 0 }, a{ 0xFF }
    {
        auto fnt = TTF_Init();
        std::cout << "Init TTF:" << (true == fnt ? "OK" : "Err");
        std::cout << std::endl;

        if(fnt)
        {
            TTF_Font* pFont = TTF_OpenFont(path.c_str(), size);
            if (pFont) 
                pData = reinterpret_cast<void*>(pFont);
            else
                std::cout << "OpenFont: Error " << std::endl;
        }
    }

    Font::~Font()
    {
        if (pData)
        {
            TTF_Font *font = reinterpret_cast<TTF_Font*>(pData);
            TTF_CloseFont(font);
        }
        
        TTF_Quit();
    }
}