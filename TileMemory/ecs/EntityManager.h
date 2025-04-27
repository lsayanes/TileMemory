

#pragma once

#include <stdint.h>
#include <vector>
#include <functional>
#include <Image.h>
#include <Entity.h>

namespace ecs
{
    using Entity_Type = Entity<format::Image, uint32_t, uint32_t, uint8_t, void*>;

    struct EntityManager : public std::vector<Entity_Type>
    {
        inline explicit EntityManager()
        {
        }

        virtual ~EntityManager() = default;

        EntityManager(const EntityManager&) = delete;
        EntityManager& operator=(const EntityManager&) = delete;
        EntityManager(EntityManager&&) = default;
        EntityManager& operator=(EntityManager&&) = default;

        Entity_Type& add(format::Image const &img) noexcept;
        void free(std::function<void(const format::Image&)> deleter);

    };
}