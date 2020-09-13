#include <Elven.h>

class TestLayer : public Elven::Layer
{
public:
    TestLayer()
        :Layer("Test")
    {
    }

    void OnUpdate() override
    {
        EL_INFO("TestLayer::Update");
    }

    void OnEvent(Elven::Event& event) override
    {
        EL_TRACE("{0}", event);
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