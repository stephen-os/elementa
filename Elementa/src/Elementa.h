#include <vector>
#include <iostream>
#include <cstdlib> // for rand()

#include "imgui.h"

#include "Lumina/Core/Layer.h"
#include "Lumina/Utils/Timer.h"

#include "Lumina/Core/Aliases.h"

#include "Lumina/Graphics/Renderer.h"
#include "Lumina/Graphics/Texture.h"
#include "Lumina/Graphics/Cameras/PerspectiveCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Elementa
{
    class Elementa : public Lumina::Layer
    {
    public:
        virtual void OnAttach() override
        {
            Lumina::Renderer::Init();

            m_Width = 400;
            m_Height = 400;

            m_Grid.resize(m_Width * m_Height, 0);
            m_PixelData.resize(m_Width * m_Height * 4, 0);

            m_Texture = Lumina::Texture::Create(m_Width, m_Height);
        }

        virtual void OnDetach() override
        {
            Lumina::Renderer::Shutdown();
        }

        virtual void OnUpdate(float timestep) override
        {
            float elapsedTime = m_FrameTimer.Elapsed();
            m_FPS = 1.0f / elapsedTime;
            m_FrameTimer.Reset();

            int spawnX = m_Width / 2;
            if (rand() % 3 == 0 && m_Grid[spawnX] == 0)
                m_Grid[spawnX] = 1;

            // Sand physics (bottom-up)
            for (int y = m_Height - 2; y >= 0; --y) {
                for (int x = 0; x < m_Width; ++x) {
                    int idx = y * m_Width + x;
                    if (m_Grid[idx] != 1)
                        continue;

                    int below = (y + 1) * m_Width + x;
                    int belowLeft = (y + 1) * m_Width + x - 1;
                    int belowRight = (y + 1) * m_Width + x + 1;

                    // Fall straight down
                    if (y + 1 < m_Height && m_Grid[below] == 0) {
                        m_Grid[below] = 1;
                        m_Grid[idx] = 0;
                        continue;
                    }

                    // Fall diagonally (randomize direction)
                    bool leftFirst = rand() % 2 == 0;
                    if (leftFirst) {
                        if (x > 0 && y + 1 < m_Height && m_Grid[belowLeft] == 0) {
                            m_Grid[belowLeft] = 1;
                            m_Grid[idx] = 0;
                        }
                        else if (x + 1 < m_Width && y + 1 < m_Height && m_Grid[belowRight] == 0) {
                            m_Grid[belowRight] = 1;
                            m_Grid[idx] = 0;
                        }
                    }
                    else {
                        if (x + 1 < m_Width && y + 1 < m_Height && m_Grid[belowRight] == 0) {
                            m_Grid[belowRight] = 1;
                            m_Grid[idx] = 0;
                        }
                        else if (x > 0 && y + 1 < m_Height && m_Grid[belowLeft] == 0) {
                            m_Grid[belowLeft] = 1;
                            m_Grid[idx] = 0;
                        }
                    }
                }
            }

            // Convert to pixel data
            for (int y = 0; y < m_Height; ++y) {
                for (int x = 0; x < m_Width; ++x) {
                    int idx = y * m_Width + x;
                    int pixelIdx = idx * 4;

                    if (m_Grid[idx] == 1) {
                        m_PixelData[pixelIdx + 0] = 194; // R
                        m_PixelData[pixelIdx + 1] = 178; // G
                        m_PixelData[pixelIdx + 2] = 128; // B
                        m_PixelData[pixelIdx + 3] = 255; // A
                    }
                    else {
                        m_PixelData[pixelIdx + 0] = 0;
                        m_PixelData[pixelIdx + 1] = 0;
                        m_PixelData[pixelIdx + 2] = 0;
                        m_PixelData[pixelIdx + 3] = 255;
                    }
                }
            }

            m_Texture->SetData(m_PixelData.data(), m_PixelData.size());
        }

        virtual void OnUIRender() override
        {
            ImGui::Begin("Viewport");
			ImVec2 viewportSize = ImGui::GetContentRegionAvail();
            ImGui::Image((void*)m_Texture->GetID(), viewportSize);
            ImGui::End();

            ImGui::Begin("Settings");
            if (ImGui::Button("Reset"))
            {
				m_Grid.clear();
				m_Grid.resize(m_Width * m_Height, 0);
                m_PixelData.clear();
				m_PixelData.resize(m_Width * m_Height * 4, 0);
                printf("Grid size: %dx%d\n", m_Width, m_Height);
            }
			ImGui::End();

            ImGui::Begin("FPS Counter");
            ImGui::Text("FPS: %.1f", m_FPS);
            ImGui::End();
        }

    private:        
        Lumina::Shared<Lumina::Texture> m_Texture; 

        std::vector<uint8_t> m_PixelData;
        std::vector<uint8_t> m_Grid;

        int m_Width = 40;
        int m_Height = 40;

        Lumina::Timer m_FrameTimer;
        float m_FPS = 0.0f;
    };
}
