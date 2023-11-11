#pragma once

namespace AnimationEngine
{
	class IContext
	{
	public:
		virtual ~IContext() = default;

		virtual void AddContext() = 0;

		virtual void ClearContext() = 0;

		virtual void SwapBuffer() = 0;

		virtual void ClearBuffer() = 0;

		virtual void ClearColor() = 0;

		virtual void EnableDepthTest(bool value) = 0;

		virtual void EnableWireFrameMode(bool value) = 0;

		virtual void EnablePointSize(bool value) = 0;
	};
}