
#include <iostream>
#include <algorithm>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Render.h"

SDL_Window*     pWindow = nullptr;
SDL_Renderer*   pRenderer = nullptr;

Render::Render(uint32_t w, uint32_t h) :
    m_uWidth{ w }, m_uHeight{ h }
    {
        auto sdl = SDL_Init(SDL_INIT_VIDEO);
        auto png = IMG_Init(IMG_INIT_PNG);

        std::cout << "Init SDL:" << (0 == sdl?"OK":SDL_GetError());
        std::cout << " PNG:" << (IMG_INIT_PNG==png?"OK":SDL_GetError());
        std::cout << std::endl;

    }

Render::~Render()
{

    std::for_each(std::begin(m_Entities), std::end(m_Entities), [&](auto const &it)
    {
        deletePng(*it.data);
    });

    m_Entities.clear();

    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
}

bool Render::createWindows(const std::string_view &title) const
{
    pWindow = SDL_CreateWindow(title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_uWidth, m_uHeight, SDL_WINDOW_SHOWN);
    pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);

    return pWindow && pRenderer;
}

void Render::draw() const
{
    std::cout << "Draw" << std::endl;
    SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
    SDL_RenderClear(pRenderer);

    std::vector<const Entity_Type>::iterator it{ m_Entities.begin() };
    while(it != m_Entities.end())
    {
        const auto img = it->data;
        SDL_Rect dest = {it->x, it->y, static_cast<int>(img->w), static_cast<int>(img->h)};

        std::cout << "Dibujando en X: " << it->x << " Y: " << it->y << " data:" << img->pData << std::endl;

        SDL_RenderCopy(pRenderer, (SDL_Texture*)(img->pData), NULL, &dest);
        it++;

    }
    SDL_RenderPresent(pRenderer);
}

void Render::doEvent() const
{
    bool bRun { true };
    SDL_Event e;

    while (bRun)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                bRun = false;
        }

        draw();
        SDL_Delay(100);
    }
}

format::Image const Render::loadPng(const std::string_view &path) const noexcept
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
        comps = surface->format->BitsPerPixel / 8;

        SDL_FreeSurface(surface); //Ya no se necesita la superficie

        std::cout << "Render create texture " << texture << std::endl;

    }

    return format::Image { w, h, comps, (const void*)texture };
}

void Render::deletePng(format::Image const &img) const noexcept
{
    std::cout << "Render destroy texture " << img.pData << std::endl;
    SDL_DestroyTexture((SDL_Texture*)(img.pData));
}

Entity_Type& Render::addEntity(const std::string_view &path) noexcept
{
    auto img = loadPng(path.data());
    std::cout << "addEntity data:" << img.pData << std::endl;
    return m_Entities.emplace_back(img.w, img.h, (const uint8_t)img.components, (const void*)img.pData);
}