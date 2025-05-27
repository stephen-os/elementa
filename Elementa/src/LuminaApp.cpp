#include "Lumina/Core/Application.h"
#include "Lumina/Core/EntryPoint.h"

#include "Elementa.h"

Lumina::Application* Lumina::CreateApplication(int argc, char** argv)
{
    Lumina::ApplicationSpecification spec;
    spec.Name = "Lumina Application";
    spec.Width = 1100;
    spec.Height = 900;
    spec.Api = API::OPENGL;

    Lumina::Application* app = new Lumina::Application(spec);
    app->PushLayer<Elementa::Elementa>();
    
    return app;
}