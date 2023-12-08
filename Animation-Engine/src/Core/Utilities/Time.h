#pragma once

namespace AnimationEngine
{
	class Time
	{
	public:
		static void Update();

		static float GetDeltaTime();

		static float GetFixedDeltaTime();

		static bool ShouldRunFixedUpdate();

		static void ResetAccumulator();

	private:
		inline static float lastFrame { 0.0f };

		inline static float deltaTime { 0.0f };

		inline static float accumulator { 0.0f };

		inline static constexpr float FIXED_TIME_STEP = 1.0f / 60.0f;
	};
}