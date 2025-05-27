#include "Water.h"

#include <cstdlib>

namespace Elementa
{
    void Water::Update(std::vector<uint8_t>& grid, int width, int height, int x, int y)
    {
        int index = y * width + x;

        if (grid[index] != 2) return; // 2 represents water

        // Below
        int below = (y + 1) * width + x;
        if (y + 1 < height && grid[below] == 0)
        {
            std::swap(grid[index], grid[below]);
            return;
        }

        // Diagonal left or right
        bool moveLeft = rand() % 2 == 0;

        int dx[2] = { -1, 1 };
        for (int i = 0; i < 2; ++i)
        {
            int dir = moveLeft ? dx[i] : dx[1 - i];
            int nx = x + dir;
            int ny = y + 1;

            if (nx >= 0 && nx < width && ny < height)
            {
                int diagIdx = ny * width + nx;
                if (grid[diagIdx] == 0)
                {
                    std::swap(grid[index], grid[diagIdx]);
                    return;
                }
            }
        }

        // Spread sideways
        for (int dir : {-1, 1})
        {
            int nx = x + dir;
            if (nx >= 0 && nx < width)
            {
                int sideIdx = y * width + nx;
                if (grid[sideIdx] == 0)
                {
                    std::swap(grid[index], grid[sideIdx]);
                    return;
                }
            }
        }
    }

    glm::u8vec4 Water::GetColor()
    {
        return glm::u8vec4(64, 164, 223, 255); // Light blue
    }
}