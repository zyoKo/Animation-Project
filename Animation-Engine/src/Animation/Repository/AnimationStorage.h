#pragma once

namespace Animator
{
	class Animation;
	
	class Model;
	
	class ITexture2D;

	struct ModelContainer
	{
		std::shared_ptr<Model> model;

		std::shared_ptr<Animation> animation;

		std::shared_ptr<ITexture2D> diffuseTexture;
	};
	
	class AnimationStorage
	{
	public:
		AnimationStorage();
	
		void AddAssetToStorage(const std::string& filename, const std::shared_ptr<ITexture2D>& textureDiffuse = nullptr);
	
		Model* GetModelForCurrentlyBoundIndex() const;
	
		Animation* GetAnimationForCurrentlyBoundIndex() const;

		ITexture2D* GetDiffuseTextureFromCurrentlyBoundIndex() const;
	
		void ChangeModel();
	
	private:
		std::shared_ptr<ModelContainer> models;

		std::vector<std::shared_ptr<Model>> modelList;
	
		std::vector<std::shared_ptr<Animation>> animationList;

		std::vector<std::shared_ptr<ITexture2D>> diffuseTextures;
	
		int currentIndex;
	};
}

