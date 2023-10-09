#include <AnimationPch.h>

#include "AnimationStorage.h"

#include "Graphics/OpenGL/Textures/ITexture2D.h"
#include "Components/Model.h"
#include "Animation/Animation.h"

namespace Animator
{
	AnimationStorage::AnimationStorage()
		: currentIndex(0)
	{
	}
	
	void AnimationStorage::AddAssetToStorage(const std::string& filename, const std::shared_ptr<ITexture2D>& textureDiffuse)
	{
		const auto model = std::make_shared<Model>(filename);
		if (textureDiffuse)
			model->SetDiffuseTextureForMeshes(textureDiffuse);
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
	
	void AnimationStorage::ChangeModel()
	{
		if (currentIndex < modelList.size() - 1)
			++currentIndex;
		else
			currentIndex = 0;
	}
}