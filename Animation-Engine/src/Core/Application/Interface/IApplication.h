#pragma once

#include <memory>

namespace AnimationEngine
{
	class IApplication
	{
	public:
		virtual ~IApplication() = default;

		virtual void Initialize() = 0;

		virtual void Update() = 0;

		virtual bool Shutdown() = 0;
	};

	std::shared_ptr<IApplication> CreateApplication();
}