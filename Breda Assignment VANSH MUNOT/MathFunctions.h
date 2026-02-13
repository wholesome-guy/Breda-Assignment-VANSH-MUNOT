#pragma once
#include <cmath>

class MathFunctions
{
public:
	static float Lerp(float _Start, float _End, float t)
	{
		return _Start + (_End - _Start) * t;
	}

	static float Cubic_Lerp(float _Start, float _End, float t)
	{
		float cubic_t = 1 - std::powf(1 - t, 3);

		return _Start + (_End - _Start) * cubic_t;
	}

	static float Quint_Lerp(float _Start, float _End, float t)
	{
		float quint_t = 1 - std::powf(1 - t, 5);

		return _Start + (_End - _Start) * quint_t;

	}

};

