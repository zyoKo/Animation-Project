#include <AnimationPch.h>

#include "AnimationStorage.h"

#include "Graphics/OpenGL/Textures/ITexture2D.h"
#include "Animation/Model.h"
#include "Animation/Animation.h"

namespace AnimationEngine
{
	AnimationStorage::AnimationStorage()
		:	currentIndex(0)
	{
	}
	
	void AnimationStorage::AddAssetToStorage(const std::string& filename, const std::shared_ptr<ITexture2D>& textureDiffuse)
	{
		const auto model = std::make_shared<Model>(filename);
		if (textureDiffuse)
		{
			model->SetDiffuseTextureForMeshes(textureDiffuse);
			diffuseTextures.push_back(textureDiffuse);
		}
		modelList.push_back(model);
	
		const auto animation = std::make_shared<Animation>(filename, model.get());
		animationList.push_back(animation);
	}
	
	Model* AnimationStorage::GetModelForCurrentlyBoundIndex() const
	{
		return modelList[currentIndex].get();
	}
	
	Animation* AnimationStorage::GetAnimationForCurrentlyBoundIndex() const
	{
		return animationList[currentIndex].get();
	}

	ITexture2D* AnimationStorage::GetDiffuseTextureFromCurrentlyBoundIndex() const
	{
		return diffuseTextures[currentIndex].get();
	}
	
	void AnimationStorage::ChangeModel()
	{
		if (currentIndex < static_cast<int>(modelList.size()) - 1)
			++currentIndex;
		else
			currentIndex = 0;
	}
}