#pragma once

#include <stdint.h>
#include <string>
#include <string_view>
#include <vector>

#include <Image.h>
#include <Entity.h>

using Entity_Type = ecs::Entity<format::Image, uint32_t, uint32_t, const uint8_t, const void*>;

class Render
{
private:
    uint32_t m_uWidth;
    uint32_t m_uHeight;

    std::vector<Entity_Type> m_Entities{};

private:
    void draw() const;

    [[nodiscard]]format::Image const loadPng(const std::string_view &path) const noexcept; //yes, return a new stack object through NVRO
    void deletePng(format::Image const &img) const noexcept;

public:

    explicit Render(uint32_t w, uint32_t h);
    virtual ~Render();

    bool createWindows(const std::string_view &title) const;
    void doEvent() const;

    Entity_Type& addEntity(const std::string_view &path) noexcept;
};
