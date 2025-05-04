#pragma once

#include <stdint.h>
#include <string>
#include <string_view>
#include <vector>

#include <Image.h>
#include <Font.h>
#include <EntityManager.h>

namespace sys
{
    class Render
    {
    private:
        uint32_t m_uWidth;
        uint32_t m_uHeight;

        std::function<void(int, int)> onTouch;

    public:

        explicit Render(uint32_t w, uint32_t h);
        virtual ~Render();

        Render(const Render&) = delete;
        Render& operator=(const Render&) = delete;
        Render(Render&&) = default;
        Render& operator=(Render&&) = default;

        [[nodiscard]] format::Image const loadPng(std::string_view path) const noexcept; //yes, return a new stack object through NVRO/VRO
        void deletePng(format::Image const& img) const noexcept;


        bool createWindows(std::string_view title) const;
        void setBackground(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 0xFF);
        void draw(ecs::EntityManager const& entities) const;
        void drawText(ecs::Entity<format::Font, std::string_view, float> const &font, const char *frmt,...) const;
        void flip();

        bool doEvent() const;


        inline void setTouch(std::function<void(int, int)> touch) { onTouch = touch; };

        inline const uint32_t w() const { return m_uWidth; };
        inline const uint32_t h() const { return m_uHeight; };
    };
}