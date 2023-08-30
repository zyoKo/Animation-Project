#pragma once

namespace Animator
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
	};
}