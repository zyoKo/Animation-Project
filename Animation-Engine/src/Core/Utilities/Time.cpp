#include <AnimationPch.h>

#include "Time.h"

#include "GLFW/glfw3.h"

namespace AnimationEngine
{
	void Time::Update()
	{
		const auto currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		accumulator += deltaTime;
	}

	float Time::GetDeltaTime()
	{
		return deltaTime;
	}

	float Time::GetFixedDeltaTime()
	{
		return FIXED_TIME_STEP;
	}

	bool Time::ShouldRunFixedUpdate()
	{
		return accumulator >= FIXED_TIME_STEP;
	}

	void Time::ResetAccumulator()
	{
		accumulator -= FIXED_TIME_STEP;
	}
}
