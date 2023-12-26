#pragma once
#include <utility>
#include <SFML/Graphics.hpp>

struct Obstacle
{
	int direction = 1;
	bool moveHorizontal = true;
	std::pair<int, int> particleIndexRange;
	std::pair<float, float> horizontalBound;
	std::pair<float, float> verticalBound;
	float movingSpeed = 1000.0f;

	Obstacle() = default;
	Obstacle(int direction, bool moveHorizontal, std::pair<int, int> particleIndexRange,
		std::pair<float, float> horizontalBound, std::pair<float, float> verticalBound, float movingSpeed)
		:direction(direction), moveHorizontal(moveHorizontal), particleIndexRange(particleIndexRange),
		horizontalBound(horizontalBound), verticalBound(verticalBound), movingSpeed(movingSpeed) {}
};
