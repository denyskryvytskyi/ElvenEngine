#include <Elven.h>

class Sandbox : public Elven::Application
{
public:
    Sandbox()
    {

    }

    ~Sandbox()
    {

    }
};

Elven::Application* Elven::CreateApplication()
{
    return new Sandbox();
}