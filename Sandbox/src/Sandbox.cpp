#include "Sandbox.h"

int main()
{
    Sandbox::SandboxApp application;

    application.Initialize();

	application.Run();

    application.Shutdown();

    return 0;
}