#pragma once

namespace Animator
{
	class IIndexBuffer
	{
	public:
		virtual ~IIndexBuffer() = default;

		virtual void Bind() = 0;

		virtual void UnBind() = 0;

		virtual void SetSize(unsigned int bufferSize) = 0;

		virtual void SetData(const void* bufferData) const = 0;
	};
}