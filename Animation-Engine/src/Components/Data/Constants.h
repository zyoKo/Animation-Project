#pragma once

#include "Math/Math.h"

namespace Animator
{
	static const std::vector<Math::Vector3F> DEFAULT_VERTICES_DATA = 
	{
		{  0.5f,  0.5f, 0.0f },
		{  0.5f, -0.5f, 0.0f },
		{ -0.5f, -0.5f, 0.0f },
		{ -0.5f,  0.5f, 0.0f }
	};

	static const std::vector<Math::Vector3F> DEFAULT_COLOR_DATA = 
	{
		{ 1.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f }
	};

	static const std::vector<Math::Vector2F> DEFAULT_TEXTURE_COORDINATES_DATA = 
	{
		{ 1.0f, 1.0f },
		{ 1.0f, 0.0f },
		{ 0.0f, 0.0f },
		{ 0.0f, 1.0f }
	};

	static const std::vector<Math::Vector3UI> DEFAULT_INDICES_DATA =
	{
		{ 0, 1, 3 },
		{ 1, 2, 3 }
	};

	// GRID DATA
	static constexpr float HALF_GRID_DIMENSIONS = 100.0f;

	static const std::vector<Math::Vector3F> GRID_VERTICES =
	{
		{  HALF_GRID_DIMENSIONS,  HALF_GRID_DIMENSIONS, 0.0f },
		{  HALF_GRID_DIMENSIONS, -HALF_GRID_DIMENSIONS, 0.0f },
		{ -HALF_GRID_DIMENSIONS, -HALF_GRID_DIMENSIONS, 0.0f },
		{ -HALF_GRID_DIMENSIONS,  HALF_GRID_DIMENSIONS, 0.0f }
	};

	static const std::vector<Math::Vector2F> GRID_TEXTURE_COORDINATES = 
	{
		{ 1.0f, 1.0f },
		{ 1.0f, 0.0f },
		{ 0.0f, 0.0f },
		{ 0.0f, 1.0f }
	};

	static const std::vector<unsigned> GRID_INDICES = { 0, 1, 3, 1, 2, 3 };
}
