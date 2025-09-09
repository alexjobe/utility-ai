#pragma once
#include <algorithm>

namespace UtilityAI
{
	inline float Clamp01(float Value) { return std::max(0.0f, std::min(1.0f, Value)); }

	inline float Normalize(float Value, float Min, float Max) 
	{
		if (Max <= Min) return 0.0f;
		return Clamp01((Value - Min) / (Max - Min));
	}
}
