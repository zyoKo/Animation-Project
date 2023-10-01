#pragma once

#include "ITexture2D.h"

namespace Animator
{
	class Texture2D : public ITexture2D
	{
	public:
		Texture2D(const void* pixels, int width, int height, int depth);

		~Texture2D() override;

		unsigned GetTextureID() const override;

		void Bind(unsigned int slot) const override;

		void UnBind() const override;

		int GetWidth() const override;

		int GetHeight() const override;

	private:
		unsigned int textureID;

		int width, height, depth;
	};
}
