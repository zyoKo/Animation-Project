#include "Core/Application/Application.h"

class Sandbox : public Animator::Application
{
public:
    Sandbox() = default;
};

int main()
{
    Sandbox application;

    application.Initialize();

	application.Run();

    application.Shutdown();

    return 0;
}