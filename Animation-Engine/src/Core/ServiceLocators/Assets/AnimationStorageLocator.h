#pragma once

namespace AnimationEngine
{
	class AnimationStorage;
}

namespace AnimationEngine
{
	class AnimationStorageLocator
	{
	public:
		static AnimationStorage* GetAnimationStorage();

		static void Provide(AnimationStorage* animationStorage);

	private:
		inline static AnimationStorage* service{ nullptr };
	};
}