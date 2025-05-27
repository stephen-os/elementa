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

#include "Elements/Sand.h"
#include "Elements/Water.h"

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
            m_PixelData.resize(m_Width * m_Height, {0.0f, 0.0f, 0.0f, 255.0f});

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
                m_Grid[spawnX] = 2;

            // Sand physics (bottom-up)
            for (int y = m_Height - 2; y >= 0; --y) {
                for (int x = 0; x < m_Width; ++x) {
                    
                    switch (m_Grid[y * m_Width + x])
                    {
					case 0:
						break;
					case 1:
						Sand::Update(m_Grid, m_Width, m_Height, x, y);
						break;
                    case 2:
						Water::Update(m_Grid, m_Width, m_Height, x, y);
						break;
                    default:
                        break;
                    }
                    
                    // Sand::Update(m_Grid, m_Width, m_Height, x, y);
                }
            }

            for (int y = 0; y < m_Height; ++y) {
                for (int x = 0; x < m_Width; ++x) {
                    int idx = y * m_Width + x;

                    switch (m_Grid[idx])
                    {
                        case 0:
                            m_PixelData[idx] = glm::u8vec4(0, 0, 0, 255); // black pixel
                            break;
						case 1:
							m_PixelData[idx] = Sand::GetColor(); // returns glm::u8vec4
							break;
						case 2:
							m_PixelData[idx] = Water::GetColor(); // returns glm::u8vec4
							break;
                    default:
                        break;
                    }
                }
            }

            m_Texture->SetData(m_PixelData.data(), m_PixelData.size() * 4);
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
                m_PixelData.resize(m_Width * m_Height, {0.0f, 0.0f, 0.0f, 255.0f});
                printf("Grid size: %dx%d\n", m_Width, m_Height);
            }
			ImGui::End();

            ImGui::Begin("FPS Counter");
            ImGui::Text("FPS: %.1f", m_FPS);
            ImGui::End();
        }

    private:        
        Lumina::Shared<Lumina::Texture> m_Texture; 
        
        std::vector<glm::u8vec4> m_PixelData;
        std::vector<uint8_t> m_Grid;

        int m_Width;
        int m_Height;

        Lumina::Timer m_FrameTimer;
        float m_FPS = 0.0f;
    };
}
