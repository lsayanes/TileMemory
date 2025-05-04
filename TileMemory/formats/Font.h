#pragma once


#pragma once

#include <stdint.h>
#include <string_view>

namespace format
{
    struct Font
    {
        void                    *pData;//data (raw or gpu texture)
        const float             size; //font size
        uint8_t                 r;
        uint8_t                 g;
        uint8_t                 b;
        uint8_t                 a;

        explicit Font(const std::string &path, float size);

        Font(const Font&) = delete;
        Font(Font&&) = delete;
        Font& operator=(const Font& o) = delete;
        Font& operator=(Font&&) = delete;


        virtual ~Font();

    };
}