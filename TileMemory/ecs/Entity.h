
#pragma once

#include <iostream>
#include <stdint.h>

namespace ecs
{
    template<typename T, typename... Ts>
    struct Entity
    {
        T *data;

        int32_t x;
        int32_t y;


        inline explicit Entity(Ts... args):
        data { new T{args...} }, x { 0 }, y { 0 }
        {
        }

        virtual ~Entity()
        {
            if(data)
                delete data;

            data = nullptr;
        }
    };
}