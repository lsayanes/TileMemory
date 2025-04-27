
#pragma once

#include <iostream>
#include <stdint.h>

namespace ecs
{
    template<typename T, typename... Ts>
    struct Entity
    {
        std::unique_ptr<T> data;

        int32_t x;
        int32_t y;

        inline explicit Entity(std::unique_ptr<T>&& ptr)
            : data{ std::move(ptr) }, x{ 0 }, y{ 0 }
        {
        }

        virtual ~Entity() = default;

        Entity(const Entity&) = delete;
        Entity& operator=(const Entity&) = delete;
        Entity(Entity&&) = default;
        Entity& operator=(Entity&&) = default;

    };
}