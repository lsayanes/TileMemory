

#pragma once

#include <stdint.h>

namespace format
{
    struct Image
    {
        const uint32_t          w; //pixels
        const uint32_t          h; //pixels
        uint8_t const           components; //RGB RGBA
        const void              *pData;   //data raw read only
        const uint32_t          size; //data size


        inline explicit Image(uint32_t width, uint32_t height, uint8_t comps, const void *data) :
            w { width }, h { height }, components{ comps },
            pData { (const void*)data }, size { width * comps * height }{ }

        virtual ~Image() = default;

        Image(const Image&) = delete;
        Image(Image&&) = delete;
        Image& operator=(const Image&) = delete;
        Image& operator=(Image&&) = delete;
    };
}