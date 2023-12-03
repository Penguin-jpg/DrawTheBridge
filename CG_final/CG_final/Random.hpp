#pragma once
#include <random>

struct RNG
{
	std::random_device rd;
	std::mt19937 generator;
	std::uniform_real_distribution<float> uniform;

	RNG() :generator(rd()), uniform(0.0f, 1.0f) {}

	float sampleUniform()
	{
		return uniform(generator);
	}

	float sampleFromRange(float min, float max)
	{
		return uniform(generator) * (max - min) + min;
	}
};