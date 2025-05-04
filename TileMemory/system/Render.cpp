
#include <iostream>
#include <algorithm>
#include <stdarg.h>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "Render.h"

namespace sys
{
    SDL_Window* pWindow = nullptr;
    SDL_Renderer* pRenderer = nullptr;


    Render::Render(uint32_t w, uint32_t h) :
        m_uWidth{ w }, m_uHeight{ h }, onTouch{ nullptr }
    {
        auto sdl = SDL_Init(SDL_INIT_VIDEO);
        
        std::cout << "Init SDL:" << (true == sdl ? "OK" : SDL_GetError());
        std::cout << std::endl;

    }

    Render::~Render()
    {
        SDL_DestroyRenderer(pRenderer);
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
    }

    bool Render::createWindows(std::string_view title) const
    {
        pWindow = SDL_CreateWindow(title.data(), m_uWidth, m_uHeight, 0);
        pWindow && (pRenderer = SDL_CreateRenderer(pWindow, NULL));

        return pWindow && pRenderer;
    }

    void Render::setBackground(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    {
        SDL_SetRenderDrawColor(pRenderer, r, g, b, a);
        SDL_RenderClear(pRenderer); //fills it with whatever color was set by SDL_SetRenderDrawColor
    }

    void Render::draw(ecs::EntityManager const& entities) const
    {
        std::vector<ecs::Entity_Type>::const_iterator it{ entities.begin() };
        while (it != entities.end())
        {
            const auto img = it->data.get();
            const SDL_FRect dest = { static_cast<float>(it->x), static_cast<float>(it->y), static_cast<float>(img->w), static_cast<float>(img->h) };

            SDL_RenderTexture(pRenderer, (SDL_Texture*)(img->pData), NULL, &dest);
            it++;

        }
    }

    void Render::drawText(ecs::Entity<format::Font, std::string_view, float> const &font, const char* frmt, ...) const
    {
        constexpr size_t len = 256;
        char str[len + 1]{""};
        va_list args;

        va_start(args, frmt);
        vsnprintf(str, len, frmt, args);
        va_end(args);

        format::Font *pFont = font.data.get();

        SDL_Color color = { pFont->r, pFont->g, pFont->b, pFont->a };
        SDL_Surface* surface = TTF_RenderText_Solid(static_cast<TTF_Font*>(pFont->pData), str, strlen(str), color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(pRenderer, surface);

        const SDL_FRect dest = { static_cast<float>(font.x), static_cast<float>(font.y), static_cast<float>(surface->w), static_cast<float>(surface->h) };
        SDL_RenderTexture(pRenderer, texture, nullptr, &dest);


        SDL_DestroySurface(surface);

    }

    void Render::flip()
    {
        SDL_RenderPresent(pRenderer);
    }

    bool Render::doEvent() const
    {
        SDL_Event e;

        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_EVENT_QUIT:
                return false;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                if (e.button.button == SDL_BUTTON_LEFT)
                {
                    if (nullptr != onTouch)
                       onTouch(static_cast<int>(e.button.x), static_cast<int>(e.button.y));
                }
             
                break;
            }
        }

        return true;
    }
    
    format::Image const Render::loadPng(std::string_view path) const noexcept
    {
        uint32_t w = 0;
        uint32_t h = 0;
        uint8_t comps = 0;

        SDL_Texture* texture = NULL;
        SDL_Surface* surface = IMG_Load(path.data());

        if (surface)
        {
            texture = SDL_CreateTextureFromSurface(pRenderer, surface);

            w = static_cast<uint32_t>(surface->w);
            h = static_cast<uint32_t>(surface->h);

            comps = SDL_BITSPERPIXEL(surface->format) / 8;


            SDL_DestroySurface(surface); //Ya no se necesita la superficie
        }

        return format::Image{ w, h, comps, (void*)texture };
    }

    void Render::deletePng(format::Image const& img) const noexcept
    {
        std::cout << "Render destroy texture " << img.pData << std::endl;
        SDL_DestroyTexture((SDL_Texture*)(img.pData));
    }



}