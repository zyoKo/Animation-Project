#pragma once

namespace Animator
{
	class Application
	{
	public:
		Application() = default;

		virtual ~Application() = default;

		virtual void Initialize();

		virtual void Update();

		virtual void Render();

		virtual void Reset();

		virtual void Shutdown();

	private:
		Application(const Application&);

		Application& operator=(const Application&);
	};
}