#pragma once

#include <memory>

namespace AnimationEngine
{
	class IApplication
	{
	public:
		virtual ~IApplication() = default;

		virtual void Initialize() = 0;

		virtual void PreUpdate() = 0;

		virtual void Update() = 0;

		virtual void PostUpdate() = 0;

		virtual void Shutdown() = 0;
	};

	std::shared_ptr<IApplication> CreateApplication();
}