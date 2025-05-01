

#pragma once

#include <stdint.h>

namespace format
{
    struct Image
    {
        const uint32_t          w; //pixels
        const uint32_t          h; //pixels
        uint8_t const           components; //RGB RGBA
        void                    *pData;   //data (raw or gpu texture)
        const uint32_t          size; //data size


        inline Image(uint32_t width, uint32_t height, uint8_t comps, void *data) :
            w { width }, h { height }, components{ comps },
            pData { data }, size { width * comps * height }{ }

        Image(const Image&) = delete;
        Image(Image&&) = delete;        
        Image& operator=(const Image& o) = delete;        
        Image& operator=(Image&&) = delete;


        virtual ~Image() = default;

    };
}