#include <AnimationPch.h>

#include "AnimationStorageLocator.h"

namespace AnimationEngine
{
	AnimationStorage* AnimationStorageLocator::GetAnimationStorage()
	{
		ANIM_ASSERT(service != nullptr, "Trying to get Animaton Storage before it was provided.");

		return service;
	}

	void AnimationStorageLocator::Provide(AnimationStorage* animationStorage)
	{
		if (animationStorage == nullptr)
			return;

		service = animationStorage;
	}
}
