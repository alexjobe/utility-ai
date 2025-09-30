#pragma once
#include "Math.h"
#include <cmath>
#include <functional>

using CurveFnSig = float(float);
using CurveFnType = std::function<CurveFnSig>;

namespace UAI
{
	struct Curves
	{
		Curves() = delete;

		inline static float Identity(float X) { return Clamp01(X); }

		inline static float Power(float X, float Gamma) 
		{ 
			return std::pow(Clamp01(X), Gamma); // Gamma < 1 inflate, > 1 compress
		}

		inline static float Smoothstep(float X) 
		{
			X = Clamp01(X);
			return X * X * (3.0f - 2.0f * X);
		}

		inline static float Logistic(float X, float X0 = 0.5f, float K = 8.0f) 
		{
			X = Clamp01(X);
			return 1.0f / (1.0f + std::exp(-K * (X - X0)));
		}

		inline static float ExpEase(float X, float K = 5.0f) 
		{
			X = Clamp01(X);
			float Denom = 1.0f - std::exp(-K);
			if (Denom <= 1e-6f) return X;
			return (1.0f - std::exp(-K * X)) / Denom;
		}

		inline static float BiasLinear(float X, float Bias = 0.5f) // 0 < Bias < 1
		{
			X = Clamp01(X);
			Bias = Clamp01(Bias);
			if (Bias == 0.5f) return X;
			return std::pow(X, std::log(Bias) / std::log(0.5f));
		}
	};
}