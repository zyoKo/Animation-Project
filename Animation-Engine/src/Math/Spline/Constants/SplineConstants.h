#pragma once

namespace AnimationEngine::Math
{
	// Curve Path Data
	static const std::vector<Vector3F> DEFAULT_CONTROL_POINTS =
	{
		{-100.0f, 1.0f,	 0.0f},
		{-80.0f,  1.0f,  40.0f},
		{-60.0f,  1.0f, -40.0f},
		{-40.0f,  1.0f,  30.0f},
		{-20.0f,  1.0f, -20.0f},
		{  0.0f,  1.0f,  50.0f},
		{ 20.0f,  1.0f, -10.0f},
		{ 40.0f,  1.0f,  20.0f},
		{ 60.0f,  1.0f, -30.0f},
		{ 80.0f,  1.0f,  10.0f},
		{100.0f,  1.0f,   0.0f}
	};
}