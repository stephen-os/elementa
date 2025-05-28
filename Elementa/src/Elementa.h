#include <vector>

#include "imgui.h"

#include "Lumina/Core/Layer.h"
#include "Lumina/Utils/Timer.h"
#include "Lumina/Core/Log.h"
#include "Lumina/Core/Input.h"

#include "Lumina/Core/Aliases.h"

#include "Lumina/Graphics/Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Elements/Element.h"
#include "Elements/Sand.h"
#include "Elements/Water.h"

namespace Elementa
{
    class Elementa : public Lumina::Layer
    {
    public:
        virtual void OnAttach() override
        {
            m_Width = 400;
            m_Height = 400;

            m_Grid.resize(m_Width * m_Height, 0);
            m_PixelData.resize(m_Width * m_Height, {0.0f, 0.0f, 0.0f, 255.0f});

            m_Texture = Lumina::Texture::Create(m_Width, m_Height);
        }

        virtual void OnDetach() override {}

        virtual void OnUpdate(float timestep) override
        {
            float elapsedTime = m_FrameTimer.Elapsed();
            m_FPS = 1.0f / elapsedTime;
            m_FrameTimer.Reset();

			// Handle mouse input and brush drawing
            if (m_IsMouseHovering && m_IsMouseDown)
            {
                float imageWidth = m_ImageMax.x - m_ImageMin.x;
                float imageHeight = m_ImageMax.y - m_ImageMin.y;

                // Convert mouse position to image-local coordinates
                float localX = (m_MousePos.x - m_ImageMin.x) / imageWidth;
                float localY = (m_MousePos.y - m_ImageMin.y) / imageHeight;

                // Convert to grid coordinates
                int gridX = static_cast<int>(localX * m_Width);
                int gridY = static_cast<int>(localY * m_Height);

                for (int offsetY = -m_BrushSize; offsetY <= m_BrushSize; ++offsetY) 
                {
                    for (int offsetX = -m_BrushSize; offsetX <= m_BrushSize; ++offsetX) 
                    {
                        int x = gridX + offsetX;
                        int y = gridY + offsetY;

                        if (x >= 0 && x < m_Width && y >= 0 && y < m_Height) 
                        {
                            float distance = glm::length(glm::vec2(offsetX, offsetY));
                            if (distance <= m_BrushSize) 
                            {
                                // Opacity controls how often we spawn sand
                                if ((rand() / float(RAND_MAX)) <= m_BrushOpacity) 
                                {
                                    int index = y * m_Width + x;
                                    if (m_Grid[index] == 0)
                                        m_Grid[index] = static_cast<uint8_t>(m_SelectedElement);
                                }
                            }
                        }
                    }
                }
            }

            for (int y = m_Height - 2; y >= 0; --y) {
                for (int x = 0; x < m_Width; ++x) {
                    
					Element element = static_cast<Element>(m_Grid[y * m_Width + x]);

                    switch (element)
                    {
                    case Element::Empyt:                                                        break; 
                    case Element::Sand:     Sand::Update(m_Grid, m_Width, m_Height, x, y);      break;
                    case Element::Water:    Water::Update(m_Grid, m_Width, m_Height, x, y);     break;
                    default:                                                                    break;
                    }
                }
            }

            for (int y = 0; y < m_Height; ++y) {
                for (int x = 0; x < m_Width; ++x) {
                    int index = y * m_Width + x;

					Element element = static_cast<Element>(m_Grid[index]);

                    switch (element)
                    {
                    case Element::Empyt:    m_PixelData[index] = glm::u8vec4(0, 0, 0, 255);     break;
                    case Element::Sand:     m_PixelData[index] = Sand::GetColor();              break;
                    case Element::Water:	m_PixelData[index] = Water::GetColor();             break;
                    default:                                                                    break;
                    }
                }
            }

            m_Texture->SetData(m_PixelData.data(), m_PixelData.size() * 4);
        }

        virtual void OnUIRender() override
        {
            ImGui::Begin("Viewport");

            ImVec2 startCursorPos = ImGui::GetCursorScreenPos();
            ImVec2 buttonSize = ImVec2(24, 24);
            float spacing = 4.0f;

            ImGui::SetCursorScreenPos(startCursorPos);

            // Render buttons for each element
            for (int i = 1; i <= static_cast<int>(Element::Water); ++i)
            {
                Element element = static_cast<Element>(i);
                glm::u8vec4 color;

                switch (element)
                {
                case Element::Sand: color = Sand::GetColor(); break;
                case Element::Water: color = Water::GetColor(); break;
                default: continue;
                }

                ImVec4 buttonColor = ImVec4(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, 1.0f);
                ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
                ImGui::PushID(i);

                if (ImGui::Button(" ", buttonSize))
                {
                    m_SelectedElement = element;
                }

                ImGui::PopID();
                ImGui::PopStyleColor();

                ImGui::SameLine(0.0f, spacing);
            }

            ImGui::NewLine();

            m_ViewportSize = ImGui::GetContentRegionAvail();
            ImGui::Image((void*)m_Texture->GetID(), m_ViewportSize);

            m_IsMouseHovering = ImGui::IsItemHovered();
            m_IsMouseDown = ImGui::IsMouseDown(ImGuiMouseButton_Left);

            m_ImageMin = ImGui::GetItemRectMin();
            m_ImageMax = ImGui::GetItemRectMax();
            m_MousePos = ImGui::GetMousePos();

            // Draw brush preview
            auto drawList = ImGui::GetWindowDrawList();
            drawList->AddCircle(ImVec2(m_MousePos.x, m_MousePos.y), m_BrushSize, IM_COL32(255, 255, 0, 128), 32, 1.5f);
            
            ImGui::End();

            ImGui::Begin("Simulation Settings");
            if (ImGui::Button("Reset Simulation"))
            {
                // Clear grid 
				m_Grid.clear();
				m_Grid.resize(m_Width * m_Height, 0);

				// Clear pixel data
                m_PixelData.clear();
                m_PixelData.resize(m_Width * m_Height, {0.0f, 0.0f, 0.0f, 255.0f});
            }
            ImGui::SliderInt("Brush Size", &m_BrushSize, 1, 50);
            ImGui::SliderFloat("Brush Opacity", &m_BrushOpacity, 0.0f, 1.0f);
			ImGui::End();

            ImGui::Begin("FPS Counter");
            ImGui::Text("FPS: %.1f", m_FPS);
            ImGui::End();
        }

    private:        
        Lumina::Shared<Lumina::Texture> m_Texture; 
        
        std::vector<glm::u8vec4> m_PixelData;
        std::vector<uint8_t> m_Grid;

        int m_BrushSize = 5;
        float m_BrushOpacity = 1.0f;

		ImVec2 m_ViewportSize = { 0, 0 };
		ImVec2 m_ImageMin = { 0, 0 };
		ImVec2 m_ImageMax = { 0, 0 };
        ImVec2 m_MousePos = { 0, 0 };

		bool m_IsMouseDown = false;
		bool m_IsMouseHovering = false;

        int m_Width;
        int m_Height;

        Element m_SelectedElement = Element::Sand;

        Lumina::Timer m_FrameTimer;
        float m_FPS = 0.0f;
    };
}
