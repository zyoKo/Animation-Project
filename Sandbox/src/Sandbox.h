#pragma once

#include <memory>

#include "Core/Application/Interface/IApplication.h"

namespace Sandbox
{
	class SandboxApp : public AnimationEngine::IApplication
	{
	public:
		void Initialize() override;

		void Update() override;

		bool Shutdown() override;
	};
}
