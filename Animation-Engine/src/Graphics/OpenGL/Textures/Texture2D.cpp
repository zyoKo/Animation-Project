#include <AnimationPch.h>

#include "Texture2D.h"

#include "Core/Logger/GLDebug.h"
#include "glad/glad.h"

namespace Animator
{
	Texture2D::Texture2D(const void* pixels, int width, int height, int depth)
		:	textureID(0),
			width(width),
			height(height),
			depth(depth)
	{
		/*GL_CALL(glCreateTextures, GL_TEXTURE_2D, 1, &textureID);
		GL_CALL(glTextureStorage2D, textureID, 1, GL_RGBA8, width, height);

		GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		GL_CALL(glTextureSubImage2D, textureID, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);*/

		GL_CALL(glCreateTextures, GL_TEXTURE_2D, 1, &textureID);
		GL_CALL(glTextureStorage2D, textureID, 1, GL_RGBA8, width, height);

		GL_CALL(glTextureParameteri, textureID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		GL_CALL(glTextureParameteri, textureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		GL_CALL(glTextureParameteri, textureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		GL_CALL(glTextureParameteri, textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		GL_CALL(glTextureSubImage2D, textureID, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &textureID);
	}

	unsigned Texture2D::GetTextureID() const
	{
		return textureID;
	}

	void Texture2D::Bind(unsigned slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, textureID);
	}

	void Texture2D::UnBind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	int Texture2D::GetWidth() const
	{
		return width;
	}

	int Texture2D::GetHeight() const
	{
		return height;
	}
}
