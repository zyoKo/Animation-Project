#pragma once

#include "Core/Application/Interface/IApplication.h"

namespace Sandbox
{
	class SandboxApp : public AnimationEngine::IApplication
	{
	public:
		void Initialize() override;

		void PreUpdate() override;

		void Update() override;

		void PostUpdate() override;

		void Shutdown() override;
	};
}
