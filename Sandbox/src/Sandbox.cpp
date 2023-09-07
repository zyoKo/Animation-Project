#include <iostream>

#include "Core/Application/Application.h"
#include "Core/Logger/Log.h"

class Sandbox : public Animator::Application
{
public:
    Sandbox() = default;
};

int main()
{
    std::unique_ptr<Sandbox> application = std::make_unique<Sandbox>();

    application->Initialize();

    auto myNumber(20);
    std::cout << typeid(myNumber).name() << std::endl;

    application->Run();

    application->Shutdown();

    return 0;
}