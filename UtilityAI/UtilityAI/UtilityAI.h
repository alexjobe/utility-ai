#pragma once
#include "Math/Curves.h"
#include "Math/Math.h"
#include <algorithm>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace UtilityAI
{
	struct CharacterContext 
	{
		std::unordered_map<std::string, float> Stats;
		std::unordered_map<std::string, float> Needs;
	};

	struct Consideration 
	{
		std::string Name;
		std::function<float(const CharacterContext&)> EvalRaw;  // Hook into game state
		CurveFn Curve = Curves::Identity;
		float MinRaw = 0.0f;
		float MaxRaw = 1.0f;
		float Weight = 1.0f;
		int CurvePriority = 0;

		float Score(const CharacterContext& Context) const 
		{
			float Raw = std::clamp(EvalRaw ? EvalRaw(Context) : 0.0f, MinRaw, MaxRaw);
			float Norm = Normalize(Raw, MinRaw, MaxRaw);
			return Curve(Norm);
		}
	};

	struct UtilityAction 
	{
		std::string Name;
		std::vector<std::string> Tags;
		std::vector<Consideration> Considerations;
	};

	struct Bias
	{
		CurveFn Curve;
		float Weight = 1.0f;
		int Priority = 0;
		std::vector<std::string> RequiredTags;
		std::function<float(const CharacterContext& Context)> EvalRaw = nullptr;
		float MinRaw = 0.0f;
		float MaxRaw = 1.0f;

		Consideration AsConsideration(const std::string& Name) const 
		{
			return { Name, EvalRaw, Curve ? Curve : Curves::Identity,
					 MinRaw, MaxRaw, Weight, Priority };
		}

		bool AppliesTo(const UtilityAction& Action) const 
		{
			if (RequiredTags.empty()) return true;
			for (auto& Tag : RequiredTags) 
			{
				if (std::find(Action.Tags.begin(), Action.Tags.end(), Tag) != Action.Tags.end())
					return true;
			}
			return false;
		}

		struct PersonalityTrait 
		{
			std::string Name;
			std::unordered_map<std::string, Bias> Biases;
		};
	};
}