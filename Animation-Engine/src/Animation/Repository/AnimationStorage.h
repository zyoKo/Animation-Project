#pragma once

namespace Animator
{
	class Animation;
	
	class Model;
	
	class ITexture2D;
	
	class AnimationStorage
	{
	public:
		AnimationStorage();
	
		void AddAssetToStorage(const std::string& filename, const std::shared_ptr<ITexture2D>& textureDiffuse = nullptr);
	
		Model* GetModelForCurrentlyBoundIndex() const;
	
		Animation* GetAnimationForCurrentlyBoundIndex() const;
	
		void ChangeModel();
	
	private:
		std::vector<std::shared_ptr<Model>> modelList;
	
		std::vector<std::shared_ptr<Animation>> animationList;
	
		int currentIndex;
	};
}

