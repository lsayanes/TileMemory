
#include <memory>
#include <algorithm>

#include "EntityManager.h"

namespace ecs
{
    Entity_Type& EntityManager::add(format::Image const& img) noexcept
    {
        auto imgPtr = std::make_unique<format::Image>(img.w, img.h, img.components, img.pData);
        auto& it = emplace_back(std::move(imgPtr));

        return it;
    }

    void EntityManager::free(std::function<void(const format::Image&)> deleter)
    {
        std::for_each(begin(), end(), [&](auto const& it)
            {
                deleter(*it.data);
            });

        clear();
    }
}