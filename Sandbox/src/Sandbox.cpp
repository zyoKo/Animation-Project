#include "Core/Application/Application.h"

class Sandbox : public Animator::Application
{
public:
    Sandbox() = default;
};

int main()
{
    std::unique_ptr<Sandbox> application = std::make_unique<Sandbox>();

    application->Update();

    return 0;
}