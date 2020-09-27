#include <Elven.h>
#include "imgui/imgui.h"

class TestLayer : public Elven::Layer
{
public:
    TestLayer()
        :Layer("Test")
    {
    }

    void OnUpdate() override
    {
        //EL_INFO("TestLayer::Update");
    }

    virtual void OnImGuiRender() override
    {
        ImGui::Begin("Test");
        ImGui::Text("Hello ImGui!");
        ImGui::End();
    }

    void OnEvent(Elven::Event& event) override
    {
        //EL_TRACE("{0}", event);
    }
};

class Sandbox : public Elven::Application
{
public:
    Sandbox()
    {
        PushLayer(new TestLayer());
    }

    ~Sandbox()
    {

    }
};

Elven::Application* Elven::CreateApplication()
{
    return new Sandbox();
}