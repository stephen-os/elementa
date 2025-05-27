#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace Elementa
{
    class Water
    {
    public:
        static void Update(std::vector<uint8_t>& grid, int width, int height, int x, int y);
        static glm::u8vec4 GetColor();
    };
}
