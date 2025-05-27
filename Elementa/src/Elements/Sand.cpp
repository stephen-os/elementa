#include "Sand.h"

#include <cstdlib>

namespace Elementa
{
    void Sand::Update(std::vector<uint8_t>& grid, int width, int height, int x, int y)
    {
        int idx = y * width + x;
        if (grid[idx] != 1)
            return;

        int below = (y + 1) * width + x;
        int belowLeft = (y + 1) * width + x - 1;
        int belowRight = (y + 1) * width + x + 1;

        if (y + 1 < height && grid[below] == 0) {
            grid[below] = 1;
            grid[idx] = 0;
            return;
        }

        bool leftFirst = rand() % 2 == 0;
        if (leftFirst) {
            if (x > 0 && y + 1 < height && grid[belowLeft] == 0) {
                grid[belowLeft] = 1;
                grid[idx] = 0;
            }
            else if (x + 1 < width && y + 1 < height && grid[belowRight] == 0) {
                grid[belowRight] = 1;
                grid[idx] = 0;
            }
        }
        else {
            if (x + 1 < width && y + 1 < height && grid[belowRight] == 0) {
                grid[belowRight] = 1;
                grid[idx] = 0;
            }
            else if (x > 0 && y + 1 < height && grid[belowLeft] == 0) {
                grid[belowLeft] = 1;
                grid[idx] = 0;
            }
        }
    }

    glm::u8vec4 Sand::GetColor()
    {
        return glm::u8vec4(194, 178, 128, 255);
    }
}